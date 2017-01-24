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
#include "file_util.h"
#include <stdlib.h>
#include <string.h>

migration_t* migration_create(const char* path, app_error_t* app_error) {
	char buf[4000] = "";
	migration_t* migration = (migration_t*) malloc( sizeof(migration_t) );
	get_basename(buf, 4000, path);
	migration->name = /*@-unrecog@*/ strdup(buf);
	migration->path = /*@-unrecog@*/ strdup(path);
	return migration;
}

void migration_run(migration_t* migration, config_t* config, app_error_t* app_error) {
	int exit_code = migration_exec(migration, config, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	if( exit_code != 0 ) {
		app_error_set(app_error, APP_ERROR_MIGRATION_FAILED, __FILE__, __LINE__, "Migration %s failed.", migration->name);
	}
}

void migration_delete(migration_t* migration, app_error_t* app_error) {
	free(migration->name);
	free(migration->path);
	free(migration);
}
