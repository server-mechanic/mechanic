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
