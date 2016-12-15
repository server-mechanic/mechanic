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
	"errors"
	"log"
	"os"
	"path"
	"strings"
	"fmt"
)

type Command struct {
	verbose         bool
	command         string
	followUpCommand []string
}

func (command *Command) printVersion() error {
	metadata := GetMetadata()
	fmt.Fprintf(os.Stderr, "This is mechanic, version: %s (built from: %s).\n", metadata.PackageVersion, metadata.ScmVersion)
	return nil
}

func (command *Command) migrate(inventory *Inventory) error {
	migrations, err := inventory.GetMigrations()
	if err != nil {
		return err
	}

	for _, migration := range migrations {
		if !migration.IsDone() {
			if err := migration.ApplyMigrationAndMarkAsDone(inventory); err != nil {
				return err
			}
		} else {
			if command.verbose {
				log.Printf("Migration %s already done.", migration.name)
			}
		}
	}

	return nil
}

func abort(exitCode int, format string, args ...interface{}) {
	log.Printf(format, args)
	os.Exit(exitCode)
}

func Run() {
	config, configErr := GetConfig()
	if configErr != nil {
		abort(1, "Reading config failed: %s", configErr)
	}

	command, err := parseArgs(config)
	if err != nil {
		abort(1, "Invalid argument(s): %s", err)
	}

	if command.command == "version" {
		command.printVersion()
	} else if command.command == "migrate" {
		initLog(config)

		inventory, inventoryErr := GetInventory(config)
		if inventoryErr != nil {
			abort(1, "Getting inventory failed: %s", inventoryErr)
		}

		if command.verbose {
			dumpConfig(config)
		}

		if err := command.migrate(inventory); err != nil {
			abort(1, "Migration failed: %s", err)
		}

		if len(command.followUpCommand) > 0 {
			if err := RunFollowUpCommand(command.followUpCommand); err != nil {
				abort(1, "Executing follow up command failed: %s", err)
			}
		}
	}

	os.Exit(0)
}

func parseArgs(config *Config) (*Command, error) {

	command := Command{}

	doubleDashSeen := false
	for i := 1; i < len(os.Args); i++ {
		arg := os.Args[i]
		hasMoreArgs := len(os.Args) > i-1
		if !doubleDashSeen && arg == "--" {
			doubleDashSeen = true
		} else if doubleDashSeen {
			command.followUpCommand = append(command.followUpCommand, arg)
		} else {
			if strings.HasPrefix(arg, "-") {
				if arg == "--verbose" || arg == "-v" {
					command.verbose = true
				} else if arg == "--logFile" || arg == "-f" {
					if !hasMoreArgs {
						return nil, errors.New("--logFile|-f requires file argument or -.")
					}
					i++
					config.logFile = os.Args[i]
				} else {
					return nil, errors.New("Unknown flag.")
				}
			} else {
				if command.command == "" {
					if arg == "migrate" || arg == "version" {
						command.command = arg
					} else {
						return nil, errors.New("Only migrate and version are allowed actions.")
					}
				} else {
					return nil, errors.New("Can execute a single command only.")
				}
			}
		}
	}

	return &command, nil
}

func dumpConfig(config *Config) {
	log.Printf("mechanic etc dir: %s\n", config.GetEtcDir())
	log.Printf("mechanic var dir: %s\n", config.GetVarDir())
	log.Printf("mechanic state dir: %s\n", config.GetStateDir())
	log.Printf("mechanic inventory db: %s\n", config.GetInventoryDbPath())
	log.Printf("mechanic migrations dir: %s\n", config.GetMigrationsDir())
}

func initLog(config *Config) {

	if config.logFile == "" || config.logFile == "-" {
		log.SetOutput(os.Stderr)
	} else {
		logFileParentPath := path.Dir(config.logFile)
		if err := os.MkdirAll(logFileParentPath, 0755); err != nil {
			abort(1, "Cannot create parent dir for log file. %s", err)
		}

		logFile, err := os.OpenFile(config.logFile, os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0660)
		if err != nil {
			abort(1, "Cannot open log file for writing. %s", err)
		}

		logWriter := bufio.NewWriter(logFile)
		log.SetOutput(logWriter)
	}
}