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

#include "mechanic/app_error.h"
#include "mechanic/log.h"
#include "string_util.h"
#include "file_util.h"
#include "mechanic/inventory.h"
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/stat.h>

typedef struct {
	int id;
	const char* name;
	const char* sql;
} db_migration_t;

static db_migration_t db_migrations[] = {
	{ 1, "initial", "create table migration ( id integer not null primary key, name text not null unique, start_time timestamp not null, end_time timestamp, status text check ( status in ( 'STARTED', 'FAILURE', 'SUCCESS' ) ) );" },
	{ 0, NULL, NULL }
};

void inventory_db_mark_migration_as_started(sqlite3* db, const char* migration_name, app_error_t* app_error) {
	int rc;
	sqlite3_stmt *stmt;

	rc = sqlite3_prepare_v2(db, "insert or replace into migration ( id, name, start_time, end_time, status ) values ( (select id from migration where name = ?), ?, strftime('%Y-%m-%d %H:%M:%f', 'now'), NULL, 'STARTED' )", -1, &stmt, 0);
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Marking migration as started failed. %s", sqlite3_errmsg(db));
		return;
	}

	rc = sqlite3_bind_text( stmt, 1, migration_name, -1, SQLITE_TRANSIENT );
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Marking migration as started failed. %s", sqlite3_errmsg(db));
		return;
	}

	rc = sqlite3_bind_text( stmt, 2, migration_name, -1, SQLITE_TRANSIENT );
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Marking migration as started failed. %s", sqlite3_errmsg(db));
		return;
	}

	rc = sqlite3_step( stmt );
	if( rc != SQLITE_DONE ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Marking migration as started failed. %s", sqlite3_errmsg(db));
		return;
	}

	sqlite3_finalize(stmt);
}

void inventory_db_mark_migration_as_failed(sqlite3* db, const char* migration_name, app_error_t* app_error) {
	int rc;
	sqlite3_stmt *stmt;

	rc = sqlite3_prepare_v2(db, "update migration set status='FAILURE', end_time=strftime('%Y-%m-%d %H:%M:%f', 'now') where name = ?", -1, &stmt, 0);
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Marking migration as failed failed. %s", sqlite3_errmsg(db));
		return;
	}

	rc = sqlite3_bind_text( stmt, 1, migration_name, -1, SQLITE_TRANSIENT );
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Marking migration as failed failed. %s", sqlite3_errmsg(db));
		return;
	}

	rc = sqlite3_step( stmt );
	if( rc != SQLITE_DONE ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Marking migration as failed failed. %s", sqlite3_errmsg(db));
		return;
	}

	sqlite3_finalize(stmt);
}

void inventory_db_mark_migration_as_succeeded(sqlite3* db, const char* migration_name, app_error_t* app_error) {
	int rc;
	sqlite3_stmt *stmt;

	rc = sqlite3_prepare_v2(db, "update migration set status='SUCCESS', end_time=strftime('%Y-%m-%d %H:%M:%f', 'now') where name = ?", -1, &stmt, 0);
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Marking migration as successful failed. %s", sqlite3_errmsg(db));
		return;
	}

	rc = sqlite3_bind_text( stmt, 1, migration_name, -1, SQLITE_TRANSIENT );
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Marking migration as successful failed. %s", sqlite3_errmsg(db));
		return;
	}

	rc = sqlite3_step( stmt );
	if( rc != SQLITE_DONE ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Marking migration as successful failed. %s", sqlite3_errmsg(db));
		return;
	}

	sqlite3_finalize(stmt);
}

bool inventory_db_is_migration_done(sqlite3* db, const char* migration_name, app_error_t* app_error) {
	int result_count;
	int rc;
	sqlite3_stmt *stmt;

	rc = sqlite3_prepare_v2(db, "SELECT count(*) FROM migration WHERE name = ? AND status = 'SUCCESS';", -1, &stmt, 0);
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Querying if migration %s has been applied failed. %s", migration_name, sqlite3_errmsg(db));
		return false;
	}

	rc = sqlite3_bind_text( stmt, 1, migration_name, -1, SQLITE_TRANSIENT );
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Querying if migration %s has been applied failed. %s", migration_name, sqlite3_errmsg(db));
		return false;
	}

	while( (rc = sqlite3_step( stmt )) ==  SQLITE_ROW ) {
		result_count = sqlite3_column_int(stmt, 0);
	}

	if( rc != SQLITE_DONE ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Querying if migration %s has been applied failed. %s", migration_name, sqlite3_errmsg(db));
		return false;
	}

	sqlite3_finalize(stmt);

	return result_count > 0;
}

static void apply_db_migration(sqlite3* db, db_migration_t* db_migration, app_error_t* app_error) {
	int rc;

	LOG_DEBUG1("Applying migration %s.", db_migration->name);

	rc = sqlite3_exec(db, db_migration->sql, NULL, NULL, NULL);
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Applying db migration %s failed. %s", db_migration->name, sqlite3_errmsg(db));
		return;
	}
}

static void insert_db_migration(sqlite3* db, db_migration_t* db_migration, app_error_t* app_error) {
	int rc;
	sqlite3_stmt *stmt;

	rc = sqlite3_prepare_v2(db, "insert into db_migration ( id, name ) values ( ?, ? )", -1, &stmt, 0);
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Inserting db migration %s failed. %s", db_migration->name, sqlite3_errmsg(db));
		return;
	}

	rc = sqlite3_bind_int( stmt, 1, db_migration->id );
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Inserting db migration %s failed. %s", db_migration->name, sqlite3_errmsg(db));
		return;
	}

	rc = sqlite3_bind_text( stmt, 2, db_migration->name, -1, SQLITE_TRANSIENT );
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Inserting db migration %s failed. %s", db_migration->name, sqlite3_errmsg(db));
		return;
	}

	rc = sqlite3_step( stmt );
	if( rc != SQLITE_DONE ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Inserting db migration %s failed. %s", db_migration->name, sqlite3_errmsg(db));
		return;
	}

	sqlite3_finalize(stmt);
}


static bool exists_table(sqlite3* db, const char* table_name, app_error_t* app_error) {
	int result_count;
	int rc;
	sqlite3_stmt *stmt;

	rc = sqlite3_prepare_v2(db, "SELECT count(*) FROM sqlite_master WHERE type='table' AND name=?;", -1, &stmt, 0);
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Query if table %s exists failed. %s", table_name, sqlite3_errmsg(db));
		return false;
	}

	rc = sqlite3_bind_text( stmt, 1, table_name, -1, SQLITE_TRANSIENT );
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Query if table %s exists failed. %s", table_name, sqlite3_errmsg(db));
		return false;
	}

	while( (rc = sqlite3_step( stmt )) ==  SQLITE_ROW ) {
		result_count = sqlite3_column_int(stmt, 0);
	}

	if( rc != SQLITE_DONE ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Query if table %s exists failed. %s", table_name, sqlite3_errmsg(db));
		return false;
	}

	sqlite3_finalize(stmt);

	return result_count > 0;
}

static void select_max_db_migration_name(sqlite3* db, char* buf, size_t buf_capacity, app_error_t* app_error) {
	int rc;
	sqlite3_stmt *stmt;

	rc = sqlite3_prepare_v2(db, "SELECT coalesce(max(name),'') FROM db_migration;", -1, &stmt, 0);
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Selecting max db migration name failed. %s", sqlite3_errmsg(db));
		return;
	}

	string_util_strcpy(buf, buf_capacity, "");
	while( (rc = sqlite3_step( stmt )) ==  SQLITE_ROW ) {
		const char* max_name = (const char*)sqlite3_column_text(stmt, 0);
		if( max_name != NULL ) {
			string_util_strcpy(buf, buf_capacity, max_name);
		}
	}

	if( rc != SQLITE_DONE ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Selecting max db migration name failed. %s", sqlite3_errmsg(db));
		return;
	}

	sqlite3_finalize(stmt);
}

static void init_db(sqlite3* db, app_error_t* app_error) {

	LOG_DEBUG("Initializing db...");
	if( !exists_table(db, "db_migration", app_error) ) {
		LOG_DEBUG("Creating table db_migration.");
	
		int rc;
		rc = sqlite3_exec(db, "create table if not exists db_migration ( id integer not null primary key, name text not null unique );", NULL, NULL, NULL);	
		if( rc != SQLITE_OK ) {
			app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Creating table db_migration failed. %s", sqlite3_errmsg(db));
			return;
		}
	}
	LOG_DEBUG("Db initialized.");
}

static void update_db(sqlite3* db, app_error_t* app_error) {
	size_t i;
	char buf[4000] = "";

	LOG_DEBUG("Updating db...");

	select_max_db_migration_name(db, buf, 4000, app_error);
	LOG_DEBUG1("Latest migration applied is %s.", buf);

	for(i=0; db_migrations[i].name != NULL; ++i) {
		if( strcmp(db_migrations[i].name, buf) > 0 ) {

			apply_db_migration(db, &db_migrations[i], app_error);	
			if( !app_error_is_ok(app_error) ) {
				return;
			}

			insert_db_migration(db, &db_migrations[i], app_error);
			if( !app_error_is_ok(app_error) ) {
				return;
			}

			LOG_DEBUG1("DB migration %s applied.", db_migrations[i].name);
		} else {
			LOG_DEBUG1("DB migration %s already applied.", db_migrations[i].name);
		}
	}

	LOG_DEBUG("Db is up to date.");
}

void inventory_db_list_migrations(sqlite3* db, list_migrations_callback_t list_migrations_callback, app_error_t* app_error) {
	int rc;
	sqlite3_stmt *stmt;

	rc = sqlite3_prepare_v2(db, "SELECT id, name, start_time, end_time, status FROM migration ORDER BY name ASC;", -1, &stmt, 0);
	if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Querying migration list failed.", sqlite3_errmsg(db));
	}

	while( (rc = sqlite3_step( stmt )) == SQLITE_ROW ) {
		int id;
		const char* name;
		const char* status;
		const char* start_time;
		const char* end_time;
		if( list_migrations_callback != NULL ) { 
			id = sqlite3_column_int(stmt, 0);
			name = (const char*)sqlite3_column_text(stmt, 1);
			start_time = (const char*)sqlite3_column_text(stmt, 2);
			end_time = (const char*)sqlite3_column_text(stmt, 3);
			status = (const char*)sqlite3_column_text(stmt, 4);
			list_migrations_callback(id, name, start_time, end_time, status);
		}
	}

	if( rc != SQLITE_DONE ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Querying migration list failed.", sqlite3_errmsg(db));
	}

	sqlite3_finalize(stmt);
}

sqlite3* inventory_db_open(const char* inventory_db_path, app_error_t* app_error) {
	sqlite3 *db = NULL;
	int rc;

	mkdirp2(inventory_db_path);

	errno = ENOERROR;
	rc = sqlite3_open(inventory_db_path, /* @in@ */ &db);
	if( db == NULL ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Opening database failed. Sqlite3 suspiciously failed to alloc mem.");
		return NULL;
	}
	else if( rc != SQLITE_OK ) {
		app_error_set(app_error, APP_ERROR_DB_ERROR, __FILE__, __LINE__, "Opening database failed. (sqlite3 message=%s, errno=%s)", sqlite3_errmsg(db), strerror(errno));
		return NULL;
	}

	init_db(db, app_error);
	if( !app_error_is_ok(app_error) ) {
		return NULL;
	}

	update_db(db, app_error);
	if( !app_error_is_ok(app_error) ) {
		return NULL;
	}

	return db;
}

void inventory_db_close(sqlite3* db, app_error_t* app_error) {
	sqlite3_close(db);
}

