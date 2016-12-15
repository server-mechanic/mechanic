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
	"bufio"
	"log"
	"os"
	"os/exec"
	"path/filepath"
)

type Migration struct {
	name string
	path string
	done bool
}

func getMigrationFilePath(migrationName string, inventory *Inventory) string {
	return filepath.Clean(inventory.config.GetMigrationsDir() + "/" + migrationName)
}

func getMigrationTempDirPath(migrationName string, inventory *Inventory) string {
	return inventory.config.GetStateDir() + "/" + migrationName + ".tmp"
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

func (migration *Migration) applyMigration(inventory *Inventory) error {

	migrationTempDirPath := getMigrationTempDirPath(migration.name, inventory)
	if err := os.MkdirAll(migrationTempDirPath, 0755); err != nil {
		return err
	}

	logFile, err := os.OpenFile(migrationTempDirPath+"/log", os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0660)
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

func (migration *Migration) IsDone() bool {
	return migration.done
}

func (migration *Migration) ApplyMigrationAndMarkAsDone(inventory *Inventory) error {
	log.Printf("Applying migration %s.\n", migration.name)

	if err := inventory.MarkMigrationStarted(migration.name); err != nil {
		return err
	}

	if err := migration.applyMigration(inventory); err != nil {
		if err2 := inventory.MarkMigrationFailed(migration.name); err2 != nil {
			return err2
		}

		return err
	} else {
		if err := inventory.MarkMigrationDone(migration.name); err != nil {
			return err
		}
	}

	return nil
}
