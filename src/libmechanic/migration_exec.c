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

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "mechanic/config.h"
#include "mechanic/app_error.h"
#include "mechanic/file_util.h"
#include "mechanic/migration.h"
#include "mechanic/log.h"
#include "mechanic/string_util.h"

extern char **environ;

static void get_migration_tmp_dir_path(char* buf, size_t buf_capacity, const char* migration_name, config_t* config, app_error_t* app_error) {
        buf[0] = 0;
        config_get_state_dir_path(config, buf, buf_capacity, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}
        string_util_strcat(buf, buf_capacity, "/");
        string_util_strcat(buf, buf_capacity, migration_name);
        string_util_strcat(buf, buf_capacity, ".tmp");
}

static void get_migration_log_file_path(char* buf, size_t buf_capacity, const char* migration_name, config_t* config, app_error_t* app_error) {
	get_migration_tmp_dir_path(buf, buf_capacity, migration_name, config, app_error);
	if( !app_error_is_ok(app_error) ) {
		return;
	}
        string_util_strcat(buf, buf_capacity, "/");
        string_util_strcat(buf, buf_capacity, "log");
}

int migration_exec(migration_t* migration, config_t* config, app_error_t* app_error)
{
	int rc;
	char migration_tmp_dir_path[4000] = "";
	char migration_log_file_path[4000] = "";
	pid_t my_pid;
	int status;
	char* argv[2];
	int log_fd;

	LOG_INFO1("Executing migration %s...", migration->name);

	get_migration_tmp_dir_path(migration_tmp_dir_path, 4000, migration->name, config, app_error);
	if( !app_error_is_ok(app_error) ) {
		return -1;
	}

	get_migration_log_file_path(migration_log_file_path, 4000, migration->name, config, app_error);
	if( !app_error_is_ok(app_error) ) {
		return -1;
	}
        mkdirp(migration_tmp_dir_path);

	argv[0] = migration->path;
	argv[1] = NULL;

	if (0 == (my_pid = fork())) {
		log_fd = open(migration_log_file_path, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);	
		dup2(log_fd, 1);
		dup2(log_fd, 2);
		close(log_fd);

		if (-1 == execve(argv[0], (char **)argv , environ)) {
			perror("child process execve failed [%m]");
			return -1;
		}
	}

	rc = waitpid(my_pid, &status, 0);
	if( rc == -1 ) {
		app_error_set(app_error, APP_ERROR_GENERIC_ERROR, __FILE__, __LINE__, "Waiting for child process of migration %s failed.", migration->name);
		return -1;
	}

	if (1 != WIFEXITED(status) || 0 != WEXITSTATUS(status)) {
		int exit_code = WEXITSTATUS(status);
		app_error_set(app_error, APP_ERROR_MIGRATION_FAILED, __FILE__, __LINE__, "Migration %s failed with exit code %d. See %s for details.", migration->name, exit_code, migration_log_file_path);

		return WEXITSTATUS(status);
	}

	LOG_DEBUG1("Removing migration tmp dir %s.", migration_tmp_dir_path);
	rmrf(migration_tmp_dir_path);

	LOG_DEBUG1("Migration %s successfully applied.", migration->name);

	return 0;
}

