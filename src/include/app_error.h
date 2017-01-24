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


#ifndef __MECHANIC_APP_ERROR_H__
#define __MECHANIC_APP_ERROR_H__

#include <stdbool.h>
#include <stdarg.h>

#define ENOERROR 0

#define APP_ERROR_OK 0
#define APP_ERROR_GENERIC_ERROR 1
#define APP_ERROR_NO_SUB_COMMAND 2
#define APP_ERROR_MIGRATION_FAILED 3
#define APP_ERROR_DB_ERROR 4
#define APP_ERROR_CONFIG_ERROR 5
#define APP_ERROR_OPT_ERROR 6

#define APP_ERROR_MESSAGE_MAX_LEN 4096

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif
#define EXIT_MIGRATION_FAILED 2

typedef struct {
	int app_errno;
	int exit_code;
	char message[APP_ERROR_MESSAGE_MAX_LEN];
	const char* file;
	int line;
} app_error_t;

void app_error_clear(app_error_t* app_error);
bool app_error_is_ok(app_error_t const* app_error);
void app_error_abort(app_error_t const* app_error);
void app_error_check(app_error_t const* app_error);
void app_error_set(app_error_t* app_error, int app_errno, const char* file, const int line, const char* format, ...);
void app_error_vset(app_error_t* app_error, int app_errno, const char* file, const int line, const char* format, va_list arg_list);

#endif
