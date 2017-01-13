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

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "log.h"
#include "app_error.h"
#include "file_util.h"
#include "config.h"
#include "command.h"
#include "constants.h"

static void init_default_log() {
	log_set_output_stream(stderr);
	log_set_log_level(false);
}

static bool starts_with(const char *a, const char *b)
{
	return ( strncmp(a, b, strlen(b)) == 0 );
}

/* finds first only */
static command_t* get_command_from(const int argc, const char** argv, app_error_t* app_error) {
	int i;

	for(i=1; i<argc; ++i) {
		if( !starts_with(argv[i], "-") ) {
			return get_command_by_name( argv[i] );
		}
	}

	return NULL;
}

static void run_command(const int argc, const char** argv, config_t* config, app_error_t* app_error) {
        command_t* command = get_command_from(argc, argv, app_error);
	if( command == NULL ) {
		command = get_help_command();
	}

        (command->command_function)(argc, argv, config, app_error);
}


static bool get_verbose_from(const int argc, const char** argv) {
	int i;

	for(i=0; i<argc; ++i) {
		if( strcmp("-v", argv[i]) == 0 ) {
			return true;
		}
	}

	return false;
}

static FILE* open_log(const int argc, const char** argv, config_t* config, app_error_t* app_error) {
	FILE* log_file = NULL;
	char cbuf[PATH_MAX_LENGTH] = "";
	bool verbose = get_verbose_from(argc, argv);

	log_set_log_level(verbose);
	config_get_log_file_path(config, cbuf, PATH_MAX_LENGTH, app_error);
	if( !app_error_is_ok(app_error) ) {
		return NULL;
	}

	LOG_DEBUG1("Log file is %s.", cbuf);
	mkdirp2(cbuf);
	log_file = fopen(cbuf, "ae");
	if( log_file == NULL ) {
		LOG_WARN2("Opening log file %s failed. %s", cbuf, strerror(errno));
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

	log_file = open_log(argc, argv, config, &app_error);
	app_error_check(&app_error);

	run_command(argc, argv, config, &app_error);
	app_error_check(&app_error);

	close_log(log_file);
	config_free(config);
	
	return 0;
}
