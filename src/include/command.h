/*
 * Copyright (c) 2016 the Server Mechanic Team.
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

#ifndef __MECHANIC_COMMAND_H__
#define __MECHANIC_COMMAND_H__

#include "app_error.h"
#include "config.h"

typedef struct {
	const char* name;
	const char* args_description;
	const char* short_description;
	void (*command_function)(const int argc, const char** argv, config_t* config, app_error_t* app_error);
} command_t;

command_t* get_command_by_name(const char* name);
command_t** get_commands();
command_t* get_help_command();

#endif
