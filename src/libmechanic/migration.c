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

#include "mechanic/migration.h"
#include "mechanic/app_error.h"
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
