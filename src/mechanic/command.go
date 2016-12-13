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
	"errors"
	"os"
	"log"
	"strings"
	"bufio"
	"path"
)

type Command struct {
	verbose         bool;
	command         string;
	followUpCommand []string;
	logFilePath     string;
}

func migrate(command *Command, inventory *Inventory) (error) {
	migrations, err := GetMigrations(inventory)
	if ( err != nil ) {
		return err
	}

	for _, migration := range migrations {
		if ( !migration.done ) {
			if err := ApplyMigrationAndMarkAsDone(&migration, inventory); err != nil {
				return err
			}
		} else {
			if ( command.verbose ) {
				log.Printf("Migration %s already done.", migration.name)
			}
		}
	}

	return nil
}

func abort(exitCode int, format string, args  ...interface{}) {
	log.Printf(format, args)
	os.Exit(exitCode)
}

func Run() {
	command, err := parseArgs()
	if err != nil {
		abort(1, "Invalid argument(s): %s", err)
	}

	if( command.logFilePath != "" && command.logFilePath != "-" ) {
		logFileParentPath := path.Dir(command.logFilePath)
		if err := os.MkdirAll(logFileParentPath, 0755); err != nil {
			abort(1, "Cannot create parent dir for log file. %s", err)
		}

		logFile, err := os.OpenFile(command.logFilePath, os.O_CREATE | os.O_WRONLY | os.O_APPEND, 0660)
		if err != nil {
			abort(1, "Cannot open log file for writing. %s", err)
		}

		logWriter := bufio.NewWriter(logFile)
		log.SetOutput(logWriter)
	}

	inventory, inventoryErr := GetInventory()
	if inventoryErr != nil {
		abort(1, "Getting inventory failed: %s", inventoryErr)
	}

	if ( command.verbose ) {
		log.Printf("mechanic etc dir: %s\n", inventory.etcDir);
		log.Printf("mechanic var dir: %s\n", inventory.varDir);
		log.Printf("mechanic state dir: %s\n", inventory.stateDir);
	}

	if err := migrate(command, inventory); err != nil {
		abort(1, "Migration failed: %s", err)
	}

	if err := RunFollowUpCommand(command.followUpCommand); err != nil {
		abort(1, "Executing follow up command failed: %s", err)
	}

	os.Exit(0)
}

func parseArgs() (*Command, error) {

	command := Command{}

	doubleDashSeen := false
	for i := 1; i < len(os.Args); i++ {
		arg := os.Args[i]
		hasMoreArgs := len(os.Args)>i-1;
		if ( !doubleDashSeen && arg == "--" ) {
			doubleDashSeen = true;
		} else if ( doubleDashSeen ) {
			command.followUpCommand = append(command.followUpCommand, arg);
		} else {
			if ( strings.HasPrefix(arg, "-") ) {
				if ( arg == "--verbose" || arg == "-v" ) {
					command.verbose = true;
				} else if ( arg == "--logFile" || arg == "-f") {
					if( !hasMoreArgs ) {
						return nil, errors.New("--logFile|-f requires file argument or -.")
					}
					i++;
					command.logFilePath = os.Args[i];
				} else {
					return nil, errors.New("Unkown flag.");
				}
			} else {
				if command.command == "" {
					if ( arg != "migrate" ) {
						return nil, errors.New("Only migrate is an allowed action.")
					}
					command.command = arg;
				} else {
					return nil, errors.New("Can execute a single command only.");
				}
			}
		}
	}

	return &command, nil
}
