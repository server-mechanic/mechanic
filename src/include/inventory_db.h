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

#ifndef __MECHANIC_INVENTORY_DB_H__
#define __MECHANIC_INVENTORY_DB_H__

#include <sqlite3.h>
#include <stdbool.h>
#include "app_error.h"
#include "migration.h"

typedef struct {
  sqlite3* db;
} inventory_db_t;

typedef enum { BY_ID, BY_START_DATE } migration_order_t;

typedef void (*inventory_db_list_migrations_callback_t)(int id, const char *name, const char* start_time, const char* end_time, const char* status);

inventory_db_t* inventory_db_open(const char* inventory_db_path, app_error_t* app_error);
void inventory_db_mark_migration_as_succeeded(inventory_db_t* inventory_db, const char* migration_name, app_error_t* app_error);
void inventory_db_mark_migration_as_failed(inventory_db_t* inventory_db, const char* migration_name, app_error_t* app_error);
void inventory_db_mark_migration_as_started(inventory_db_t* inventory_db, const char* migration_name, app_error_t* app_error);
bool inventory_db_is_migration_done(inventory_db_t* inventory_db, const char* migration_name, app_error_t* app_error);
void inventory_db_list_migrations(inventory_db_t* inventory_db, migration_order_t order, inventory_db_list_migrations_callback_t callback, app_error_t* app_error);
void inventory_db_close(inventory_db_t* db, app_error_t* app_error);

#endif
