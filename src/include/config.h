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

#ifndef __MECHANIC_CONFIG_H__
#define __MECHANIC_CONFIG_H__

#include <stdio.h>
#include <stdlib.h>
#include "app_error.h"
#include "file_util.h"

#define CONFIG_MAX_KEY_LENGTH 100
#define CONFIG_MAX_VALUE_LENGTH 4000

#define CONFIG_KEY_LOG_FILE "log_file"
#define CONFIG_KEY_MIGRATION_DIRS "migration_dirs"
#define CONFIG_KEY_STATE_DIR "state_dir"
#define CONFIG_KEY_RUN_DIR "run_dir"

typedef struct {
	char log_file_path[PATH_MAX_LENGTH];
	char migration_dirs_path[PATH_MAX_LENGTH];
	char state_dir[PATH_MAX_LENGTH];
	char run_dir[PATH_MAX_LENGTH];
} config_t;

config_t* config_get(app_error_t* app_error);
void config_free(config_t* config);
void config_get_log_file_path(config_t* config, char* buf, size_t buf_cap, app_error_t* app_error);
void config_get_state_dir_path(config_t* config, char* buf, size_t buf_cap, app_error_t* app_error);
void config_get_run_dir(config_t* config, char* buf, size_t buf_cap, app_error_t* app_error);
void config_get_migration_dirs_path(config_t* config, char* buf, size_t buf_cap, app_error_t* app_error);
void config_get_etc_dir_path(config_t* config, char* buf, size_t buf_cap, app_error_t* app_error);
void config_get_inventory_db_path(config_t* config, char* cbuf, size_t buf_cap, app_error_t* app_error);

typedef void (*config_key_value_func_t)(config_t* config, const char* key, const char* value, app_error_t* app_error);
void config_parse(config_t* config, FILE* in, config_key_value_func_t key_value_handler, app_error_t* app_error);

#endif
