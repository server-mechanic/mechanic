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


#include "metadata.h"
#include "app_error.h"
#include "config.h"
#include "command.h"
#include <stdio.h>
#include <string.h>

void print_usage(const int argc, const char** argv, config_t* config, app_error_t* app_error) {
	command_t** commands = get_commands();
	int i=0;

	printf("usage: mechanic [-v] <command> [ <args> ]\n");
	printf("  -v\tLog more information.\n");
	printf("command:\n");
	for(i=0; commands[i] != NULL; ++i) {
		printf("  %s %s - %s\n", commands[i]->name, commands[i]->args_description, commands[i]->short_description);
	}
}
