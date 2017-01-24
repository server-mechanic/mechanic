/*
 * Copyright (c) 2016,2017 the Server Mechanic Team.
 * Server Mechanic (http://server-mechanic.org)
 *
 * This file is part of Server Mechanic.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#ifndef __MECHANIC_CONFIG_H__
#define __MECHANIC_CONFIG_H__

#include <stdio.h>
#include <stdlib.h>
#include "app_error.h"
#include "file_util.h"

#define CONFIG_MAX_KEY_LENGTH 100
#define CONFIG_MAX_VALUE_LENGTH 4000
#define CONFIG_MAX_SECTION_LENGTH 100

#define CONFIG_SECTION_MAIN "main"
#define CONFIG_KEY_LOG_FILE "log_file"
#define CONFIG_KEY_MIGRATION_DIRS "migration_dirs"
#define CONFIG_KEY_PRE_MIGRATION_DIRS "pre_migration_dirs"
#define CONFIG_KEY_POST_MIGRATION_DIRS "post_migration_dirs"
#define CONFIG_KEY_STATE_DIR "state_dir"
#define CONFIG_KEY_RUN_DIR "run_dir"

typedef struct {
	char log_file_path[PATH_MAX_LENGTH];
	char migration_dirs_path[PATH_MAX_LENGTH];
	char pre_migration_dirs_path[PATH_MAX_LENGTH];
	char post_migration_dirs_path[PATH_MAX_LENGTH];
	char state_dir[PATH_MAX_LENGTH];
	char run_dir[PATH_MAX_LENGTH];
} config_t;

config_t* config_get(app_error_t* app_error);
void config_free(config_t* config);
void config_get_log_file_path(config_t* config, char* buf, size_t buf_cap, app_error_t* app_error);
void config_get_state_dir_path(config_t* config, char* buf, size_t buf_cap, app_error_t* app_error);
void config_get_run_dir(config_t* config, char* buf, size_t buf_cap, app_error_t* app_error);
void config_get_migration_dirs_path(config_t* config, char* buf, size_t buf_cap, app_error_t* app_error);
void config_get_pre_migration_dirs_path(config_t* config, char* buf, size_t buf_cap, app_error_t* app_error);
void config_get_post_migration_dirs_path(config_t* config, char* buf, size_t buf_cap, app_error_t* app_error);
void config_get_etc_dir_path(config_t* config, char* buf, size_t buf_cap, app_error_t* app_error);
void config_get_inventory_db_path(config_t* config, char* cbuf, size_t buf_cap, app_error_t* app_error);

typedef void (*config_key_value_func_t)(config_t* config, const char* section, const char* key, const char* value, app_error_t* app_error);
void config_parse(config_t* config, FILE* in, config_key_value_func_t key_value_handler, app_error_t* app_error);

#endif
