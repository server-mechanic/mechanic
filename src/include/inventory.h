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


#ifndef __MECHANIC_INVENTORY_H__
#define __MECHANIC_INVENTORY_H__

#include <stdbool.h>
#include "app_error.h"
#include "config.h"
#include "migration.h"
#include "inventory_db.h"

typedef struct {
	inventory_db_t* db;
	config_t* config;
} inventory_t;

typedef void (*list_migrations_callback_t)(int id, const char *name, const char* start_time, const char* end_time, const char* status);

inventory_t* inventory_open(config_t* config, app_error_t* app_error);
void inventory_close(inventory_t* inventory, app_error_t* app_error);
void inventory_collect_migrations(inventory_t* inventory, migration_list_t* list, app_error_t* app_error);
void inventory_collect_pre_migrations(inventory_t* inventory, migration_list_t* list, app_error_t* app_error);
void inventory_collect_post_migrations(inventory_t* inventory, migration_list_t* list, app_error_t* app_error);
void inventory_mark_migration_started(inventory_t* inventory, migration_t* migration, app_error_t* app_error);
void inventory_mark_migration_as_done(inventory_t* inventory, migration_t* migration, app_error_t* app_error);
void inventory_mark_migration_as_failed(inventory_t* inventory, migration_t* migration, app_error_t* app_error);
void inventory_list_migrations(inventory_t* inventory, migration_order_t order, list_migrations_callback_t list_migrations_callback, app_error_t* app_error);

#endif
