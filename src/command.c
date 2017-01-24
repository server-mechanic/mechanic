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


#include "command.h"
#include "app_error.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

extern void print_version(const int argc, const char** argv, config_t* config, app_error_t* app_error);
extern void print_usage(const int argc, const char** argv, config_t* config, app_error_t* app_error);
extern void migrate(const int argc, const char** argv, config_t* config, app_error_t* app_error);
extern void list_migrations(const int argc, const char** argv, config_t* config, app_error_t* app_error);

static command_t version_command = { "version", "", "Print mechanic version information.", print_version };
static command_t migrate_command = { "migrate", "[-- <follow up command> [ <args> ] ]", "Apply migrations.", migrate };
static command_t list_migrations_command = { "list-migrations", "", "List migrations.", list_migrations };
static command_t help_command = { "help", "", "Print usage information.", print_usage };

static command_t* commands[] = { &migrate_command, &list_migrations_command, &version_command, &help_command, NULL };

command_t** get_commands() {
	return commands;
}

command_t* get_help_command() {
	return &help_command;
}

command_t* get_command_by_name(const char* name) {
	int i;

	for(i=0; commands[i] != NULL; ++i) {
		if( strcmp(name, commands[i]->name) == 0 ) {
			return commands[i];
		}
	}

	return NULL;
}
