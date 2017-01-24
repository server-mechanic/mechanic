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


#include "inventory.h"
#include "app_error.h"
#include "migration.h"
#include "follow_up_command.h"
#include <stdio.h>

static void apply_migrations(inventory_t* inventory, config_t* config, migration_list_t* migration_list, app_error_t* app_error) {
	size_t i;
	migration_t* migration;

	for(i=0; i<migration_list->length; ++i) {
		migration = migration_list->migrations[i];
		inventory_mark_migration_started(inventory, migration, app_error);

		migration_run(migration, config, app_error);
		if( app_error_is_ok(app_error) ) {
			inventory_mark_migration_as_done(inventory, migration, app_error);
		}
		else
		{
			inventory_mark_migration_as_failed(inventory, migration, app_error);
		}
	}
}

void migrate(const int argc, const char** argv, config_t* config, app_error_t* app_error) {

	inventory_t* inventory = inventory_open(config, app_error);
	migration_list_t* migration_list = NULL;
	migration_list_t* pre_migration_list = NULL;
	migration_list_t* post_migration_list = NULL;

	migration_list = migration_list_alloc(8, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	pre_migration_list = migration_list_alloc(8, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	post_migration_list = migration_list_alloc(8, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	inventory_collect_pre_migrations(inventory, pre_migration_list, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	inventory_collect_migrations(inventory, migration_list, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	inventory_collect_post_migrations(inventory, post_migration_list, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	apply_migrations(inventory, config, pre_migration_list, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	apply_migrations(inventory, config, migration_list, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	apply_migrations(inventory, config, post_migration_list, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	if( app_error_is_ok(app_error) ) {
		run_follow_up_command(argc, argv, app_error);
	}

	migration_list_free(migration_list, app_error);
	migration_list_free(pre_migration_list, app_error);
	migration_list_free(post_migration_list, app_error);
	inventory_close(inventory, app_error);
} 
