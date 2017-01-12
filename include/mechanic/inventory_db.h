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
#include "mechanic/app_error.h"
#include "mechanic/migration.h"

typedef void (*inventory_db_list_migrations_callback_t)(int id, const char *name, const char* start_time, const char* end_time, const char* status);

sqlite3* inventory_db_open(const char* inventory_db_path, app_error_t* app_error);
void inventory_db_mark_migration_as_succeeded(sqlite3* db, const char* migration_name, app_error_t* app_error);
void inventory_db_mark_migration_as_failed(sqlite3* db, const char* migration_name, app_error_t* app_error);
void inventory_db_mark_migration_as_started(sqlite3* db, const char* migration_name, app_error_t* app_error);
bool inventory_db_is_migration_done(sqlite3* db, const char* migration_name, app_error_t* app_error);
void inventory_db_list_migrations(sqlite3* db, inventory_db_list_migrations_callback_t callback, app_error_t* app_error);
void inventory_db_close(sqlite3* db, app_error_t* app_error);

#endif
