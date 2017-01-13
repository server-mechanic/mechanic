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
