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
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "mechanic/app_error.h"
#include "mechanic/config.h"
#include "mechanic/constants.h"
#include "mechanic/log.h"
#include "mechanic/string_util.h"

static void get_expanded_path(char* buf, size_t buf_cap, const char* value, /*@unused@*/ app_error_t* app_error) {
	const char* rootDirEnv = getenv("MECHANIC_ROOT_DIR");
	string_util_strcpy(buf, buf_cap, value );
	if( NULL != rootDirEnv ) {
		string_util_replace(buf, buf_cap, "${MECHANIC_ROOT_DIR}", rootDirEnv);
	} else {
		string_util_replace(buf, buf_cap, "${MECHANIC_ROOT_DIR}", "");
	}
}

void config_get_etc_dir_path(/*@unused@*/ config_t* config, char* buf, size_t buf_cap, app_error_t* app_error) {
	get_expanded_path(buf, buf_cap, "${MECHANIC_ROOT_DIR}/etc/mechanic", app_error );
}

void config_get_inventory_db_path(/*@unused@*/ config_t* config, char* buf, size_t buf_cap, app_error_t* app_error) {
	get_expanded_path(buf, buf_cap, "${MECHANIC_ROOT_DIR}/var/lib/mechanic/inventory.db", app_error );
}

static void get_conf_file_path(char* buf, size_t buf_cap, app_error_t* app_error) {
	get_expanded_path(buf, buf_cap, "${MECHANIC_ROOT_DIR}/etc/mechanic.conf", app_error );
}

void config_get_state_dir_path(/*@unused@*/ config_t* config, char* buf, size_t buf_cap, app_error_t* app_error) {
	get_expanded_path(buf, buf_cap, "${MECHANIC_ROOT_DIR}/var/lib/mechanic/state", app_error );
}

void config_get_migration_dirs_path( config_t* config, char* buf, size_t buf_cap, app_error_t* app_error) {
	get_expanded_path(buf, buf_cap, config->migration_dirs_path, app_error);
}

void config_get_log_file_path(/*@unused@*/ config_t* config, char* buf, size_t buf_cap, app_error_t* app_error) {
	const char* path = config->log_file_path;
	if( NULL == path ) {
		get_expanded_path(buf, buf_cap, "${MECHANIC_ROOT_DIR}/var/log/mechanic.log", app_error );
	} else {
		get_expanded_path(buf, buf_cap, path, app_error );
	}
}

static void handle_key_value(config_t* config, const char* key, const char* value, /*@unused@*/ app_error_t* app_error) {
	if( strcmp(CONFIG_KEY_LOG_FILE, key) == 0 ) {
		string_util_strcpy(config->log_file_path, PATH_MAX_LENGTH, value);
	}
	else if( strcmp(CONFIG_KEY_MIGRATION_DIRS, key) == 0 ) {
		string_util_strcpy(config->migration_dirs_path, PATH_MAX_LENGTH, value);
	} else {
		LOG_INFO1("Unknown config option %s.", key);
	}
}

static void read_config(config_t* config, app_error_t* app_error) {
	char cbuf[PATH_MAX_LENGTH] = "";
	FILE* conf_file;
	
	get_conf_file_path(cbuf, PATH_MAX_LENGTH, app_error);
	LOG_DEBUG1("Config file is %s.", cbuf);
	conf_file = fopen(cbuf, "r");
	if( conf_file != NULL ) {
		config_parse( config, conf_file, handle_key_value, app_error);
	}
	else {
		handle_key_value( config, CONFIG_KEY_LOG_FILE, "${MECHANIC_ROOT_DIR}/var/log/mechanic.log", app_error);
		handle_key_value( config, CONFIG_KEY_MIGRATION_DIRS, "${MECHANIC_ROOT_DIR}/etc/mechanic/migration.d:${MECHANIC_ROOT_DIR}/var/lib/mechanic/migration.d", app_error );
	}
}

config_t* config_get(app_error_t* app_error) {
	config_t* config = malloc( sizeof(config_t) );
	string_util_strcpy(config->log_file_path, PATH_MAX_LENGTH, "/dev/stderr");

	read_config(config, app_error);
	return config;
}

void config_free(config_t* config) {
	free(config);
}
