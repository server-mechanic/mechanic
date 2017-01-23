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

#include "inventory.h"
#include "app_error.h"
#include "migration.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define UNNULL(s) ((s) == NULL ? "<NULL>" : (s))

static void print_migration(int id, const char* name, const char* start_time, const char* end_time, const char* status) {
	printf("%d\t%s\t%s\t%s\t%s\n", id, name, UNNULL(start_time), UNNULL(end_time), UNNULL(status));
}

static const char* get_opt_if_defined(const int argc, const char** argv, const char* flag) {
	for(int i=0; i<argc; ++i) {
		if( strstr(argv[i], flag) == argv[i] ) {
			return argv[i];
		}
	}

	return NULL;
}

static migration_order_t get_order(const int argc, const char** argv, app_error_t* app_error) {
	const char* order_opt = get_opt_if_defined(argc, argv, "--order-by=");

	if( order_opt == NULL ) {
		return BY_START_DATE;
	}

	if( strcmp(order_opt, "--order-by=start_date") == 0 ) {
		return BY_START_DATE;
	} else if( strcmp(order_opt, "--order-by=id") == 0 ) {
		return BY_ID;
	}

	app_error_set(app_error, APP_ERROR_OPT_ERROR, __FILE__, __LINE__, "Invalid order-by opt: %s Try --order-by=id or --order-by=start_date.", order_opt);

	return BY_ID;
}

void list_migrations(const int argc, const char** argv, config_t* config, app_error_t* app_error) {
	migration_order_t order = BY_START_DATE;

	inventory_t* inventory = inventory_open(config, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	order = get_order(argc, argv, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	inventory_list_migrations(inventory, order, print_migration, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	inventory_close(inventory, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}
}
