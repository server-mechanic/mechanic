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
#include <stdio.h>

void print_version(const int argc, const char** argv, config_t* config, app_error_t* app_error) {
	printf("Version: %s, SCM-Version: %s\n", VERSION, SCM_VERSION);
} 
