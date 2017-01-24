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

