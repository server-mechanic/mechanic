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

#include "mechanic/metadata.h"
#include "mechanic/app_error.h"
#include "mechanic/config.h"
#include "mechanic/command.h"
#include <stdio.h>
#include <string.h>

void print_usage(const int argc, const char** argv, config_t* config, app_error_t* app_error) {
	command_t** commands = get_commands();
	int i=0;

	printf("usage: mechanic [-v] <command> [ <args> ]\n");
	printf("  -v\tLog more information.\n");
	printf("command:\n");
	for(i=0; commands[i] != NULL; ++i) {
		printf("  %s %s - %s\n", commands[i]->name, commands[i]->args_description, commands[i]->short_description);
	}
}
