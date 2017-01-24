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
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define UNNULL(s) ((s) == NULL ? "<NULL>" : (s))

static void print_migration(int id, const char* name, const char* start_time, const char* end_time, const char* status) {
	printf("%d\t%s\t%s\t%s\t%s\n", id, name, UNNULL(start_time), UNNULL(end_time), UNNULL(status));
}

static const char* get_opt_if_defined(const int argc, const char** argv, const char* flag) {
	int i;

	for(i=0; i<argc; ++i) {
		if( strstr(argv[i], flag) == argv[i] ) {
			return argv[i];
		}
	}

	return NULL;
}

static migration_order_t get_order_opt(const int argc, const char** argv, app_error_t* app_error) {
	const char* order_opt = get_opt_if_defined(argc, argv, "--order-by=");

	if( order_opt == NULL ) {
		return BY_START_DATE;
	}

	if( strcmp(order_opt, "--order-by=start_date") == 0 ) {
		return BY_START_DATE;
	} else if( strcmp(order_opt, "--order-by=id") == 0 ) {
		return BY_ID;
	}

	app_error_set(app_error, APP_ERROR_OPT_ERROR, __FILE__, __LINE__, "Invalid --order-by value: \"%s\". Valid ones are: id, start_date.", order_opt);
	return BY_ID;
}

void list_migrations(const int argc, const char** argv, config_t* config, app_error_t* app_error) {
	migration_order_t order = BY_START_DATE;

	inventory_t* inventory = inventory_open(config, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	order = get_order_opt(argc, argv, app_error);
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
