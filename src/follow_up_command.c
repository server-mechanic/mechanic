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


#include "follow_up_command.h"
#include "app_error.h"
#include "log.h"
#include <unistd.h>
#include <string.h>

extern char** environ;

static int get_follow_up_command_start(const int argc, const char** argv) {
	int i;

	for(i=0; i<argc; ++i) {
		if( strcmp(argv[i], "--") == 0 && i < argc-1 ) {
			return i+1;
		}
	}

	return -1;
}

void run_follow_up_command(const int argc, const char** argv, app_error_t* app_error) {
	int fup_cmd = get_follow_up_command_start(argc, argv);
	if( fup_cmd != -1 ) {
		if (-1 == execve(argv[fup_cmd], (char **)&argv[fup_cmd], environ) ) {
			app_error_set(app_error, APP_ERROR_GENERIC_ERROR, __FILE__, __LINE__, "Executing follow up command %s failed.", argv[fup_cmd]);
			return;
		}
	}
}
