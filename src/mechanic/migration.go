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
	"os/exec"
	"os"
	"path/filepath"
	"log"
	"bufio"
)

type Migration struct {
	name string;
	path string;
	done bool;
}

func getMigrationFilePath(migrationName string, inventory *Inventory) (string) {
	return filepath.Clean(getMigrationDirPath(inventory) + "/" + migrationName)
}

func getMigrationDirPath(inventory *Inventory) (string) {
	return filepath.Join(inventory.etcDir + "/migration.d/")
}

func getMigrationTempDirPath(migrationName string, inventory *Inventory) (string) {
	return inventory.stateDir + "/" + migrationName + ".tmp";
}

func runWithLog(cmdPath string, logFile *os.File) error {

	logWriter := bufio.NewWriter(logFile)
	cmd := exec.Command(cmdPath)
	cmd.Stdin = nil
	cmd.Stdout = logWriter
	cmd.Stderr = logWriter
	defer logWriter.Flush()

	if err := cmd.Run(); err != nil {
		return err
	}

	return nil
}

func applyMigration(migration *Migration, inventory *Inventory) (error) {

	migrationTempDirPath := getMigrationTempDirPath(migration.name, inventory);
	if err := os.MkdirAll(migrationTempDirPath, 0755); err != nil {
		return err
	}

	logFile, err := os.OpenFile(migrationTempDirPath + "/log", os.O_CREATE | os.O_WRONLY | os.O_APPEND, 0660)
	if err != nil {
		return err
	}

	migrationFilePath := getMigrationFilePath(migration.name, inventory)
	if err := runWithLog(migrationFilePath, logFile); err != nil {
		return err
	}

	err = os.RemoveAll(migrationTempDirPath)
	if err != nil {
		return err
	}

	return nil
}

func ApplyMigrationAndMarkAsDone(migration *Migration, inventory *Inventory) (error) {
	log.Printf("Applying migration %s.\n", migration.name)

	if err := applyMigration(migration, inventory); err != nil {
		return err
	} else {
		if err := markMigrationDone(migration.name, inventory); err != nil {
			return err
		}
	}

	return nil
}

func markMigrationDone(migrationName string, inventory*Inventory) (error) {
	migrationDoneFilePath := getMigrationDonePath(migrationName, inventory)

	if err := createFile(migrationDoneFilePath); err != nil {
		return err
	}
	return nil
}

func getMigrationDonePath(migrationName string, inventory *Inventory) (string) {
	return inventory.stateDir + "/" + migrationName + ".done";
}

func createFile(path string) (error) {
	file, err := os.Create(path)
	if err != nil {
		return err
	}

	defer file.Close()

	return nil
}
