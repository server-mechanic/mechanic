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
#include "mechanic/config.h"
#include "mechanic/file_util.h"
#include "mechanic/string_util.h"
#include "mechanic/log.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/stat.h>

static inventory_t inventory;

static void get_migration_done_file_path(config_t* config, char* buf, size_t buf_capacity, const char* migration_name, app_error_t* app_error) {
	config_get_state_dir_path(config, buf, buf_capacity, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}
	string_util_strcat(buf, buf_capacity, "/");
	string_util_strcat(buf, buf_capacity, migration_name);
	string_util_strcat(buf, buf_capacity, ".done");
}

static void init_dirs(config_t* config, app_error_t* app_error) {
	char path[1000] = "";
	
	config_get_state_dir_path(config, path, 1000, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}
	mkdirp(path);

	config_get_etc_dir_path(config, path, 1000, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}
	mkdirp(path);
}

inventory_t* inventory_open(config_t* config, app_error_t* app_error) {
	init_dirs(config, app_error);
	if( !app_error_is_ok(app_error) ) {
		return NULL;
	}

	inventory.db = inventory_db_open(config, app_error);
	if( !app_error_is_ok(app_error) ) {
		return NULL;
	}

	inventory.config = config;

	return &inventory;
}

void inventory_close(inventory_t* inventory, app_error_t* app_error) {
	inventory_db_close(inventory->db, app_error);
	inventory->db = NULL;
	inventory->config = NULL;
}

bool is_migration_done(inventory_t* inventory, char* migration_name, app_error_t* app_error) {
	char buf[4000] = "";

	get_migration_done_file_path(inventory->config, buf, 4000, migration_name, app_error);
	if( is_file(buf) ) {
		return true;
	}

	return inventory_db_is_migration_done(inventory->db, migration_name, app_error);
}

void inventory_mark_migration_started(inventory_t* inventory, migration_t* migration, app_error_t* app_error) {
	inventory_db_mark_migration_as_started(inventory->db, migration->name, app_error);
}

static void create_migration_done_file(config_t* config, const char* migration_name, app_error_t* app_error) {
	char cbuf[4000] = "";
	config_get_state_dir_path(config, cbuf, 4000, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}

	string_util_strcat(cbuf, 4000, "/");
	string_util_strcat(cbuf, 4000, migration_name);
	string_util_strcat(cbuf, 4000, ".done");
	mkdirp2(cbuf);

	create_file(cbuf, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}
}

void inventory_mark_migration_as_done(inventory_t* inventory, migration_t* migration, app_error_t* app_error) {
	create_migration_done_file(inventory->config, migration->name, app_error);
	inventory_db_mark_migration_as_succeeded(inventory->db, migration->name, app_error);
}

void inventory_mark_migration_as_failed(inventory_t* inventory, migration_t* migration, app_error_t* app_error) {
	inventory_db_mark_migration_as_failed(inventory->db, migration->name, app_error);
}

static int compare_migrations_by_name(const void* a, const void* b) {
	const migration_t* migration_a = *(migration_t**)a;
	const migration_t* migration_b = *(migration_t**)b;
	int result;
	result = strcmp(migration_a->name, migration_b->name);
	return result;
}

void inventory_collect_migrations(inventory_t* inventory, migration_list_t* list, app_error_t* app_error) {
	size_t i;
	int rc;
	migration_t* migration = NULL;
	DIR *dir;
	struct dirent *ent;
	char cbuf[4000] = "";
	char path_buf[4000] = "";

	config_get_migrations_dir_path(inventory->config, cbuf, 4000, app_error);
	LOG_DEBUG1("Migrations dir is %s.", cbuf);
	dir = opendir(cbuf);
	if( dir != NULL ) {
		while( (ent=readdir(dir)) != NULL) {
			if( ent->d_type == DT_REG ) {
				if( !is_migration_done(inventory, ent->d_name, app_error) ) {
					string_util_strcpy(path_buf, 4000, cbuf);
					string_util_strcat(path_buf, 4000, "/");
					string_util_strcat(path_buf, 4000, ent->d_name);

					migration = migration_create(path_buf, app_error);
					migration_list_add_migration(list, migration, app_error);
				} else {
					LOG_DEBUG1("Migration %s already marked as done. Skipped.", ent->d_name);
				}
			}
		}
		rc = closedir(dir);
		if( rc != 0 ) {
			LOG_DEBUG2("Closing migrations dir %s failed. %s", cbuf, strerror(errno));
		}
	}

	qsort(list->migrations, list->length, sizeof(migration_t*), compare_migrations_by_name);
	LOG_DEBUG1("%d migration(s) pending...", list->length);
	for(i=0; i<list->length; ++i) {
		LOG_DEBUG2("#%d: %s", i, list->migrations[i]->name);
	}
}
