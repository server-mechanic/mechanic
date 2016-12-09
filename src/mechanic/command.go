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
)

type Command struct {
	verbose         bool;
	command         string;
	followUpCommand []string;
}

func migrate(command *Command, inventory *Inventory) (error) {
	migrations, err := GetMigrations(inventory)
	if( err != nil ) {
		return err
	}

	for _, migration := range migrations {
		if( !migration.done ) {
			if err := ApplyMigrationAndMarkAsDone(&migration, inventory); err != nil {
				return err
			}
		} else {
			if( command.verbose ) {
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

	inventory, inventoryErr := GetInventory()
	if inventoryErr != nil {
		abort(1, "Getting inventory failed: %s", inventoryErr)
	}

	if ( command.verbose ) {
		log.Printf("mechanic etc dir:   %s\n", inventory.etcDir);
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
	for _, arg := range os.Args[1:] {
		if ( !doubleDashSeen && arg == "--" ) {
			doubleDashSeen = true;
		} else if ( doubleDashSeen ) {
			command.followUpCommand = append(command.followUpCommand, arg);
		} else {
			if ( strings.HasPrefix(arg, "-") ) {
				if ( arg == "--verbose" || arg == "-v" ) {
					command.verbose = true;
				} else {
					return nil, errors.New("Unkown flag.");
				}
			} else {
				if command.command == "" {
					if( arg != "migrate" ) {
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
