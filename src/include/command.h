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


#ifndef __MECHANIC_COMMAND_H__
#define __MECHANIC_COMMAND_H__

#include "app_error.h"
#include "config.h"

typedef struct {
	const char* name;
	const char* args_description;
	const char* short_description;
	void (*command_function)(const int argc, const char** argv, config_t* config, app_error_t* app_error);
} command_t;

command_t* get_command_by_name(const char* name);
command_t** get_commands();
command_t* get_help_command();

#endif
