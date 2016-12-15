/*
 * Copyright (c) 2016 Cornelius Buschka.
 * Server Mechanic (http://server-mechanic.org)
 *
 * This file is part of Server Mechanic.
 *
 * Server Mechanic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
package mechanic

import (
	"database/sql"
	_ "github.com/mattn/go-sqlite3"
	"io/ioutil"
	"os"
	"path"
)

type Inventory struct {
	varDir   string
	stateDir string
	etcDir   string
	db       *sql.DB
}

func GetMigrations(inventory *Inventory) ([]Migration, error) {
	migrations := make([]Migration, 0)

	migrationDir := path.Join(inventory.etcDir, "migration.d")
	files, _ := ioutil.ReadDir(migrationDir)
	for _, f := range files {
		migrationPath := path.Join(migrationDir, f.Name())

		migrationDone, err := isMigrationDone(f.Name(), inventory)
		if err != nil {
			return nil, err
		}

		migration := Migration{path: migrationPath, name: f.Name(), done: migrationDone}
		migrations = append(migrations, migration)
	}

	return migrations, nil
}

func GetInventory() (*Inventory, error) {
	inventory := Inventory{
		varDir:   "/var/lib/mechanic/",
		stateDir: "/var/lib/mechanic/state",
		etcDir:   "/etc/mechanic",
		db:       nil}

	envRootDir := os.Getenv("MECHANIC_ROOT_DIR")
	if envRootDir != "" {
		inventory.etcDir = path.Clean(path.Join(envRootDir, inventory.etcDir))
		inventory.stateDir = path.Clean(path.Join(envRootDir, inventory.stateDir))
		inventory.varDir = path.Clean(path.Join(envRootDir, inventory.varDir))
	}

	envEtcDir := os.Getenv("MECHANIC_ETC_DIR")
	if envEtcDir != "" {
		inventory.etcDir = path.Clean(envEtcDir)
	}

	envVarDir := os.Getenv("MECHANIC_VAR_DIR")
	if envVarDir != "" {
		inventory.varDir = path.Clean(envVarDir)
	}

	envStateDir := os.Getenv("MECHANIC_STATE_DIR")
	if envStateDir != "" {
		inventory.stateDir = path.Clean(envStateDir)
	}

	if err := initDirs(&inventory); err != nil {
		return nil, err
	}

	db, err := initDb(inventory.varDir + "/inventory.db")
	if err != nil {
		return nil, err
	}

	inventory.db = db

	return &inventory, nil
}

func initDb(path string) (*sql.DB, error) {
	db, err := sql.Open("sqlite3", path)
	if err != nil {
		return nil, err
	}

	createSql := `create table db_version (
		id integer not null primary key
	);
	insert into db_version ( id ) values ( 1 );

	create table migration (
		id integer not null primary key,
		name text not null unique,
		start_time timestamp not null,
		end_time timestamp,
		status text check ( status in ( 'STARTED', 'FAILURE', 'SUCCESS' ) )
	);`
	_, err = db.Exec(createSql)
	if err != nil {
		return nil, err
	}

	return db, err
}

func initDirs(inventory *Inventory) error {
	if err := os.MkdirAll(inventory.etcDir, 0755); err != nil {
		return err
	}

	if err := os.MkdirAll(inventory.stateDir, 0755); err != nil {
		return err
	}

	return nil
}

func isMigrationDone(migrationName string, inventory *Inventory) (bool, error) {

	if done, err := existsMigrationDoneFile(migrationName, inventory); done || err != nil {
		return done, err
	}

	return isMigrationDoneInDb(migrationName, inventory)
}

func existsMigrationDoneFile(migrationName string, inventory *Inventory) (bool, error) {
	migrationDoneFilePath := getMigrationDonePath(migrationName, inventory)

	_, err := os.Stat(migrationDoneFilePath)
	if err == nil {
		return true, nil
	}
	if os.IsNotExist(err) {
		return false, nil
	}
	return true, err
}

func updateMigrationAsDone(migrationName string, inventory *Inventory) error {

	updateSql := "update migration set status='SUCCESS', end_time=strftime('%Y-%m-%d %H:%M:%f', 'now') where name = ?"
	stmt, err := inventory.db.Prepare(updateSql)
	if err != nil {
		return err
	}
	defer stmt.Close()

	_, err2 := stmt.Exec(migrationName)
	if err2 != nil {
		return err2
	}

	return nil
}

func insertMigrationStartEvent(migrationName string, inventory *Inventory) error {

	insertSql := `insert or replace into migration ( id, name, start_time, end_time, status )
		values ( (select id from migration where name = ?), ?, strftime('%Y-%m-%d %H:%M:%f', 'now'), NULL, 'STARTED' )`
	stmt, err := inventory.db.Prepare(insertSql)
	if err != nil {
		return err
	}
	defer stmt.Close()

	_, err2 := stmt.Exec(migrationName, migrationName)
	if err2 != nil {
		return err2
	}

	return nil
}

func isMigrationDoneInDb(migrationName string, inventory *Inventory) (bool, error) {

	querySql := "select count(*) from migration where name=? and status='SUCCESS'"
	stmt, err := inventory.db.Prepare(querySql)
	if err != nil {
		return false, err
	}
	defer stmt.Close()

	row := stmt.QueryRow(migrationName)

	var count int
	err2 := row.Scan(&count)
	if err2 != nil {
		return false, err
	}

	return count > 0, nil
}

func MarkMigrationStarted(migrationName string, inventory *Inventory) error {
	return insertMigrationStartEvent(migrationName, inventory)
}

func MarkMigrationDone(migrationName string, inventory *Inventory) error {
	if err := createMigrationDoneFile(migrationName, inventory); err != nil {
		return err
	}

	return updateMigrationAsDone(migrationName, inventory)
}

func MarkMigrationFailed(migrationName string, inventory *Inventory) error {

	updateSql := "update migration set status='FAILURE', end_time=strftime('%Y-%m-%d %H:%M:%f', 'now') where name=?"
	stmt, err := inventory.db.Prepare(updateSql)
	if err != nil {
		return err
	}
	defer stmt.Close()

	_, err2 := stmt.Exec(migrationName)
	if err2 != nil {
		return err2
	}

	return nil
}

func createMigrationDoneFile(migrationName string, inventory *Inventory) error {
	migrationDoneFilePath := getMigrationDonePath(migrationName, inventory)

	if err := createFile(migrationDoneFilePath); err != nil {
		return err
	}
	return nil
}

func getMigrationDonePath(migrationName string, inventory *Inventory) string {
	return inventory.stateDir + "/" + migrationName + ".done"
}

func createFile(path string) error {
	file, err := os.Create(path)
	if err != nil {
		return err
	}

	defer file.Close()

	return nil
}
