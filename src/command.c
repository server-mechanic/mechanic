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

#include "mechanic/command.h"
#include "mechanic/app_error.h"
#include "mechanic/config.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

extern void print_version(const int argc, const char** argv, config_t* config, app_error_t* app_error);
extern void print_usage(const int argc, const char** argv, config_t* config, app_error_t* app_error);
extern void migrate(const int argc, const char** argv, config_t* config, app_error_t* app_error);
extern void list_migrations(const int argc, const char** argv, config_t* config, app_error_t* app_error);

static command_t version_command = { "version", "", "Print mechanic version information.", print_version };
static command_t migrate_command = { "migrate", "[-- <follow up command> [ <args> ] ]", "Apply migrations.", migrate };
static command_t list_migrations_command = { "list-migrations", "", "List migrations.", list_migrations };
static command_t help_command = { "help", "", "Print usage information.", print_usage };

static command_t* commands[] = { &migrate_command, &list_migrations_command, &version_command, &help_command, NULL };

command_t** get_commands() {
	return commands;
}

command_t* get_help_command() {
	return &help_command;
}

command_t* get_command_by_name(const char* name) {
	int i;

	for(i=0; commands[i] != NULL; ++i) {
		if( strcmp(name, commands[i]->name) == 0 ) {
			return commands[i];
		}
	}

	return NULL;
}
