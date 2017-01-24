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


#ifndef __MECHANIC_MIGRATION_H__
#define __MECHANIC_MIGRATION_H__

#include "app_error.h"
#include "config.h"

typedef struct {
	char* name;
	char* path;
} migration_t;

migration_t* migration_create(const char* path, app_error_t* app_error);
void migration_run(migration_t* migration, config_t* config, app_error_t* app_error);
void migration_delete(migration_t* migration, app_error_t* app_error);
int migration_exec(migration_t* migration, config_t* config, app_error_t* app_error);

typedef struct {
	size_t length;
	size_t capacity;
	migration_t** migrations;
} migration_list_t;

migration_list_t* migration_list_alloc(size_t capacity, app_error_t* app_error);
void migration_list_add_migration(migration_list_t* migration_list, migration_t* migration, app_error_t* app_error);
void migration_list_free(migration_list_t* migration_list, app_error_t* app_error);

#endif
