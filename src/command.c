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

typedef struct {
	const char* name;
	void (*command_function)(const int argc, const char** argv, config_t* config, app_error_t* app_error);
} sub_command_t;

static sub_command_t sub_commands[] = {
	{ "version", print_version },
	{ "migrate", migrate },
	{ NULL, NULL }
};

static bool starts_with(const char *a, const char *b)
{
   if( strncmp(a, b, strlen(b)) == 0 ) 
	return 1;

   return 0;
}

static sub_command_t* get_sub_command_by_name(const char* name) {
	int i;

	for(i=0; sub_commands[i].name != NULL; ++i) {
		if( strcmp(name, sub_commands[i].name) == 0 ) {
			return &sub_commands[i];
		}
	}

	return NULL;
}

/* finds first only */
static sub_command_t* get_sub_command_from(const int argc, const char** argv, app_error_t* app_error) {
	int i;
	sub_command_t* sub_command = NULL;

	for(i=0; i<argc; ++i) {
		if( !starts_with(argv[i], "-") ) {
			sub_command = get_sub_command_by_name( argv[i] );
			if( sub_command != NULL ) {
				return sub_command;
			}
		}
	}

	app_error_set(app_error, APP_ERROR_NO_SUB_COMMAND, __FILE__, __LINE__, "No sub command found.");

	return NULL;
}

void run_command(const int argc, const char** argv, config_t* config, app_error_t* app_error) {
	sub_command_t* sub_command = get_sub_command_from(argc, argv, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	(sub_command->command_function)(argc, argv, config, app_error);
}
