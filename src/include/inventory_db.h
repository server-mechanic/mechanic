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
