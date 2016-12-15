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
	config *Config
	db     *sql.DB
}

func (inventory *Inventory) GetMigrations() ([]Migration, error) {
	migrations := make([]Migration, 0)

	migrationsDir := inventory.config.GetMigrationsDir()
	files, _ := ioutil.ReadDir(migrationsDir)
	for _, f := range files {
		migrationPath := path.Join(migrationsDir, f.Name())

		migrationDone, err := inventory.isMigrationDone(f.Name())
		if err != nil {
			return nil, err
		}

		migration := Migration{path: migrationPath, name: f.Name(), done: migrationDone}
		migrations = append(migrations, migration)
	}

	return migrations, nil
}

func GetInventory(config *Config) (*Inventory, error) {
	inventory := Inventory{
		db:     nil,
		config: config}

	if err := inventory.initDirs(); err != nil {
		return nil, err
	}

	db, err := inventory.initDb()
	if err != nil {
		return nil, err
	}

	inventory.db = db

	return &inventory, nil
}

func (inventory *Inventory) initDb() (*sql.DB, error) {
	inventoryDbPath := inventory.config.GetInventoryDbPath()

	db, err := sql.Open("sqlite3", inventoryDbPath)
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

func (inventory *Inventory) initDirs() error {
	if err := os.MkdirAll(inventory.config.GetEtcDir(), 0755); err != nil {
		return err
	}

	if err := os.MkdirAll(inventory.config.GetStateDir(), 0755); err != nil {
		return err
	}

	return nil
}

func (inventory *Inventory) isMigrationDone(migrationName string) (bool, error) {

	if done, err := inventory.existsMigrationDoneFile(migrationName); done || err != nil {
		return done, err
	}

	return inventory.isMigrationDoneInDb(migrationName)
}

func (inventory *Inventory) existsMigrationDoneFile(migrationName string) (bool, error) {
	migrationDoneFilePath := inventory.getMigrationDonePath(migrationName)

	_, err := os.Stat(migrationDoneFilePath)
	if err == nil {
		return true, nil
	}
	if os.IsNotExist(err) {
		return false, nil
	}
	return true, err
}

func (inventory *Inventory) updateMigrationAsDone(migrationName string) error {

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

func (inventory *Inventory) insertMigrationStartEvent(migrationName string) error {

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

func (inventory *Inventory) isMigrationDoneInDb(migrationName string) (bool, error) {

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

func (inventory *Inventory) MarkMigrationStarted(migrationName string) error {
	return inventory.insertMigrationStartEvent(migrationName)
}

func (inventory *Inventory) MarkMigrationDone(migrationName string) error {
	if err := inventory.createMigrationDoneFile(migrationName); err != nil {
		return err
	}

	return inventory.updateMigrationAsDone(migrationName)
}

func (inventory *Inventory) MarkMigrationFailed(migrationName string) error {

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

func (inventory *Inventory) createMigrationDoneFile(migrationName string) error {
	migrationDoneFilePath := inventory.getMigrationDonePath(migrationName)

	if err := createFile(migrationDoneFilePath); err != nil {
		return err
	}
	return nil
}

func (inventory *Inventory) getMigrationDonePath(migrationName string) string {
	return inventory.config.GetStateDir() + "/" + migrationName + ".done"
}

func createFile(path string) error {
	file, err := os.Create(path)
	if err != nil {
		return err
	}

	defer file.Close()

	return nil
}
