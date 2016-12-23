/*
 * Copyright (c) 2016 Cornelius Buschka.
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

#ifndef __MECHANIC_STRING_UTIL_H__
#define __MECHANIC_STRING_UTIL_H__

#include <stdlib.h>
#include <string.h>

void string_util_strcpy(char* buf, size_t buf_capacity, const char* str);
void string_util_strcat(char* buf, size_t buf_capacity, const char* str);
void string_util_replace(char *buf, size_t buf_capacity, const char *old_str, const char *new_str);

#endif
