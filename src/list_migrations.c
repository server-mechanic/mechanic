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

#include "mechanic/inventory.h"
#include "mechanic/app_error.h"
#include "mechanic/migration.h"
#include <stdio.h>

#define UNNULL(s) ((s) == NULL ? "<NULL>" : (s))

static void print_migration(int id, const char* name, const char* start_time, const char* end_time, const char* status) {
	printf("%d\t%s\t%s\t%s\t%s\n", id, name, UNNULL(start_time), UNNULL(end_time), UNNULL(status));
}

void list_migrations(const int argc, const char** argv, config_t* config, app_error_t* app_error) {

	inventory_t* inventory = inventory_open(config, app_error);

	inventory_list_migrations(inventory, print_migration, app_error);

	inventory_close(inventory, app_error);
}
