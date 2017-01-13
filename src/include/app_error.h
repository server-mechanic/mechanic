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

#ifndef __MECHANIC_APP_ERROR_H__
#define __MECHANIC_APP_ERROR_H__

#include <stdbool.h>

#define ENOERROR 0

#define APP_ERROR_OK 0
#define APP_ERROR_GENERIC_ERROR 1
#define APP_ERROR_NO_SUB_COMMAND 2
#define APP_ERROR_MIGRATION_FAILED 3
#define APP_ERROR_DB_ERROR 4
#define APP_ERROR_CONFIG_ERROR 4

#define APP_ERROR_MESSAGE_MAX_LEN 4096

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif
#define EXIT_MIGRATION_FAILED 2

typedef struct {
	int app_errno;
	int exit_code;
	char message[APP_ERROR_MESSAGE_MAX_LEN];
	const char* file;
	int line;
} app_error_t;

void app_error_clear(app_error_t* app_error);
bool app_error_is_ok(app_error_t* app_error);
void app_error_abort(app_error_t* app_error);
void app_error_check(app_error_t* app_error);
void app_error_set(app_error_t* app_error, int app_errno, const char* file, const int line, const char* format, ...);

#endif
