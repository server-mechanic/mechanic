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
#include "inventory_db.h"
#include "config.h"
#include "file_util.h"
#include "string_util.h"
#include "log.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/stat.h>

static inventory_t inventory;

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
}

inventory_t* inventory_open(config_t* config, app_error_t* app_error) {
	char cbuf[4000] = "";

	init_dirs(config, app_error);
	if( !app_error_is_ok(app_error) ) {
		return NULL;
	}

	config_get_inventory_db_path(config, cbuf, 4000, app_error);
	if( !app_error_is_ok(app_error) ) {
		return NULL;
	}

	inventory.db = new InventoryDb();
	inventory.db->inventory_db_open(cbuf, app_error);
	if( !app_error_is_ok(app_error) ) {
		return NULL;
	}

	inventory.config = config;

	return &inventory;
}

void inventory_close(inventory_t* inventory, app_error_t* app_error) {
	inventory->db->inventory_db_close(app_error);
	delete inventory->db;
	inventory->db = NULL;
	inventory->config = NULL;
}

void inventory_list_migrations(inventory_t* inventory, migration_order_t order, list_migrations_callback_t list_migrations_callback, app_error_t* app_error)
{
	inventory->db->inventory_db_list_migrations(order, list_migrations_callback, app_error);
}

bool is_migration_done(inventory_t* inventory, char* migration_name, app_error_t* app_error) {
	return inventory->db->inventory_db_is_migration_done(migration_name, app_error);
}

void inventory_mark_migration_started(inventory_t* inventory, migration_t* migration, app_error_t* app_error) {
	inventory->db->inventory_db_mark_migration_as_started(migration->name, app_error);
}

void inventory_mark_migration_as_done(inventory_t* inventory, migration_t* migration, app_error_t* app_error) {
	inventory->db->inventory_db_mark_migration_as_succeeded(migration->name, app_error);
}

void inventory_mark_migration_as_failed(inventory_t* inventory, migration_t* migration, app_error_t* app_error) {
	inventory->db->inventory_db_mark_migration_as_failed(migration->name, app_error);
}

static int compare_migrations_by_name(const void* a, const void* b) {
	const migration_t* migration_a = *(migration_t**)a;
	const migration_t* migration_b = *(migration_t**)b;
	int result;
	result = strcmp(migration_a->name, migration_b->name);
	return result;
}

static void collect_migrations_from_dir(inventory_t* inventory, const char* dir_path, migration_list_t* list, bool check_if_applied, app_error_t* app_error) {
	DIR *dir;
	struct dirent *ent;
	char path_buf[4000] = "";
	int rc;
	migration_t* migration = NULL;

	dir = opendir(dir_path);
	if( dir == NULL ) {
		LOG_DEBUG2("Could not collect migrations from %s. (%s)", dir_path, strerror(errno));
		return;
	}

	while( (ent=readdir(dir)) != NULL) {
		if( ent->d_type == DT_REG ) {
			if( !check_if_applied || !is_migration_done(inventory, ent->d_name, app_error) ) {
				string_util_strcpy(path_buf, 4000, dir_path);
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
		LOG_DEBUG2("Closing migrations dir %s failed. %s", dir_path, strerror(errno));
	}
}

static void collect_migrations(inventory_t* inventory, const char* dirs_path, migration_list_t* list, bool check_if_applied, app_error_t* app_error) {
	char path_buf[4000] = "";
	size_t pos;
	size_t i;

	for(pos=0; pos<strlen(dirs_path); pos=pos+strlen(path_buf)+1 ) {
		string_util_substring(path_buf, 4000, &dirs_path[pos], ':');
		collect_migrations_from_dir(inventory, path_buf, list, check_if_applied, app_error);
	}

	qsort(list->migrations, list->length, sizeof(migration_t*), compare_migrations_by_name);
	LOG_DEBUG1("%d migration(s) pending...", list->length);
	for(i=0; i<list->length; ++i) {
		LOG_DEBUG2("#%d: %s", i, list->migrations[i]->name);
	}
}

void inventory_collect_migrations(inventory_t* inventory, migration_list_t* list, app_error_t* app_error) {
	char cbuf[8000] = "";

	config_get_migration_dirs_path(inventory->config, cbuf, 8000, app_error);
	LOG_DEBUG1("Migrations dirs path is %s.", cbuf);

	collect_migrations(inventory, cbuf, list, true, app_error);
}

void inventory_collect_pre_migrations(inventory_t* inventory, migration_list_t* list, app_error_t* app_error) {
	char cbuf[8000] = "";

	config_get_pre_migration_dirs_path(inventory->config, cbuf, 8000, app_error);
	LOG_DEBUG1("Migrations dirs path is %s.", cbuf);

	collect_migrations(inventory, cbuf, list, false, app_error);
}

void inventory_collect_post_migrations(inventory_t* inventory, migration_list_t* list, app_error_t* app_error) {
	char cbuf[8000] = "";

	config_get_post_migration_dirs_path(inventory->config, cbuf, 8000, app_error);
	LOG_DEBUG1("Migrations dirs path is %s.", cbuf);

	collect_migrations(inventory, cbuf, list, false, app_error);
}
