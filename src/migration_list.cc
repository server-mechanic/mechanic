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

#include "migration.h"
#include "app_error.h"
#include <stdlib.h>
#include <string.h>

migration_list_t* migration_list_alloc(size_t capacity, app_error_t* app_error) {
	migration_list_t* list = (migration_list_t*) malloc(sizeof(migration_list_t) );
	list->capacity = capacity;
	list->length = 0;
	list->migrations = (migration_t**) malloc(sizeof(migration_t*) * capacity); 

	// TODO check return

	return list;
}

void migration_list_free(migration_list_t* list, app_error_t* app_error) {
	free(list->migrations);
	if( list != NULL )
		list->migrations = NULL;	
	free(list);
}

void migration_list_add_migration(migration_list_t* list, migration_t* migration, app_error_t* app_error) {
	migration_t** old_migrations = NULL;

	if( list->capacity > list->length ) {
		list->migrations[list->length] = migration;
		list->length = list->length+1;
	}
	else
	{
		old_migrations = list->migrations;
		list->migrations = (migration_t**) malloc(sizeof(migration_t*) * list->capacity*2);
		memcpy(list->migrations, old_migrations, sizeof(migration_t*)*list->length );
		list->capacity = list->capacity*2;
		list->migrations[list->length] = migration;
		list->length = list->length+1;
		free(old_migrations);
	}
}

