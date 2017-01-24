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


#ifndef __MECHANIC_FILE_UTIL_H__
#define __MECHANIC_FILE_UTIL_H__

#include <stdbool.h>
#include <stdlib.h>

#define PATH_MAX_LENGTH 4000

bool is_file(const char* path);
void rmrf(const char* path);
void mkdirp(const char* path);
void mkdirp2(const char* path);
void get_basename(char* buf, size_t buf_capacity, const char* path);
int create_file(const char* path);

#endif
