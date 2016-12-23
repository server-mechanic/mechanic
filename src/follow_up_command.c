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

#include "mechanic/follow_up_command.h"
#include "mechanic/app_error.h"
#include "mechanic/log.h"
#include <unistd.h>
#include <string.h>

extern char** environ;

static int get_follow_up_command_start(const int argc, const char** argv) {
	int i;

	for(i=0; i<argc; ++i) {
		if( strcmp(argv[i], "--") == 0 && i < argc-1 ) {
			return i+1;
		}
	}

	return -1;
}

void run_follow_up_command(const int argc, const char** argv, app_error_t* app_error) {
	int fup_cmd = get_follow_up_command_start(argc, argv);
	if( fup_cmd != -1 ) {
		if (-1 == execve(argv[fup_cmd], (char **)&argv[fup_cmd], environ) ) {
			app_error_set(app_error, APP_ERROR_GENERIC_ERROR, __FILE__, __LINE__, "Executing follow up command %s failed.", argv[fup_cmd]);
			return;
		}
	}
}
