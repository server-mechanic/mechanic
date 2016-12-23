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

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mechanic/log.h"
#include "mechanic/app_error.h"
#include "mechanic/file_util.h"
#include "mechanic/config.h"
#include "mechanic/command.h"
#include "mechanic/constants.h"

static void init_default_log() {
	log_set_output_stream(stderr);
	log_set_log_level(false);
}

static FILE* open_log(config_t* config, app_error_t* app_error) {
	FILE* log_file = NULL;
	char cbuf[PATH_MAX_LENGTH] = "";

	config_get_log_file_path(config, cbuf, PATH_MAX_LENGTH, app_error);
	app_error_check(app_error);

	LOG_DEBUG1("Log file is %s.", cbuf);
	mkdirp2(cbuf);
	log_file = fopen(cbuf, "a");
	if( log_file == NULL ) {
		app_error_set(app_error, APP_ERROR_GENERIC_ERROR, __FILE__, __LINE__, "Opening log file %s failed.", cbuf);
		return NULL;
	}

	return log_file;
}

static void close_log(FILE* log_file) {
	if( log_file != NULL ) {
		fclose(log_file);
	}
}

int main(const int argc, const char** argv) {
	FILE* log_file;
	app_error_t app_error;
	config_t* config;

	app_error_clear(&app_error);

	init_default_log();

	config = config_get(&app_error);
	app_error_check(&app_error);

	log_file = open_log(config, &app_error);
	app_error_check(&app_error);

	run_command(argc, argv, config, &app_error);
	app_error_check(&app_error);

	close_log(log_file);
	config_free(config);
	
	return 0;
}
