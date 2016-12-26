/*
 * Copyright (c) 2016 Cornelius Buschka.
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

#include "mechanic/inventory.h"
#include "mechanic/app_error.h"
#include "mechanic/migration.h"
#include "mechanic/follow_up_command.h"
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

	migration_list = migration_list_alloc(8, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	inventory_collect_migrations(inventory, migration_list, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	apply_migrations(inventory, config, migration_list, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	if( app_error_is_ok(app_error) ) {
		run_follow_up_command(argc, argv, app_error);
	}

	migration_list_free(migration_list, app_error);
	inventory_close(inventory, app_error);
} 
