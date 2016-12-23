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

#include "mechanic/app_error.h"
#include "mechanic/log.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

/*@-formatconst@ */ void app_error_set(app_error_t* app_error, int app_errno, const char* file, const int line, const char* format, ...) {
	char cbuf[4000] = "";

	va_list arg_list;

	app_error->app_errno = app_errno;
	app_error->message[0] = (char)0; 
	app_error->file = file;
	app_error->line = line;
	if( app_errno == APP_ERROR_OK ) {
		return;
	}
	else if( app_errno == APP_ERROR_GENERIC_ERROR ) {
		va_start(arg_list, format);
		(void)vsnprintf(cbuf, 4000, format, arg_list);
		va_end(arg_list);
		(void)snprintf(app_error->message, APP_ERROR_MESSAGE_MAX_LEN, "%s (%s)", cbuf, strerror(errno));
	}
	else {
		va_start(arg_list, format);
		(void)vsnprintf(app_error->message, APP_ERROR_MESSAGE_MAX_LEN, format, arg_list);
		va_end(arg_list);
	}
}

void app_error_clear(app_error_t* app_error) {
	app_error_set(app_error, APP_ERROR_OK, "none", -1, "It's all ok.");
}

bool app_error_is_ok(app_error_t* app_error) {
	return app_error != NULL && app_error->app_errno == APP_ERROR_OK;
}

void app_error_check(app_error_t* app_error) {
	if( app_error_is_ok(app_error) ) {
		return;
	}

	app_error_abort(app_error);
}

void app_error_abort(app_error_t* app_error) {
	log_error(app_error->file, app_error->line, app_error->message);
	exit(EXIT_FAILURE);
}
