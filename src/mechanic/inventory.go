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
	"os"
	"path"
	"io/ioutil"
)

type Inventory struct {
	stateDir        string;
	etcDir          string;
}

func GetMigrations(inventory *Inventory) ([]Migration, error) {
	migrations := make([]Migration, 0)

	migrationDir := path.Join(inventory.etcDir, "migration.d")
	files, _ := ioutil.ReadDir(migrationDir);
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
	inventory := Inventory{stateDir: "/var/lib/mechanic/state", etcDir: "/etc/mechanic"}

	envRootDir := os.Getenv("MECHANIC_ROOT_DIR");
	if ( envRootDir != "" ) {
		inventory.etcDir = path.Clean(path.Join(envRootDir, inventory.etcDir));
		inventory.stateDir = path.Clean(path.Join(envRootDir, inventory.stateDir));
	}

	envEtcDir := os.Getenv("MECHANIC_ETC_DIR");
	if ( envEtcDir != "" ) {
		inventory.etcDir = path.Clean(envEtcDir);
	}

	envStateDir := os.Getenv("MECHANIC_STATE_DIR");
	if ( envStateDir != "" ) {
		inventory.stateDir = path.Clean(envStateDir);
	}

	if err := initDirs(&inventory); err != nil {
		return nil, err
	}

	return &inventory, nil
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
	migrationDoneFilePath := getMigrationDonePath(migrationName, inventory)

	_, err := os.Stat(migrationDoneFilePath)
	if err == nil {
		return true, nil;
	}
	if os.IsNotExist(err) {
		return false, nil
	}
	return true, err
}
