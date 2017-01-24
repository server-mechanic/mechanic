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


#ifndef __MECHANIC_STRING_UTIL_H__
#define __MECHANIC_STRING_UTIL_H__

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void string_util_substring(char* buf, size_t buf_capacity, const char* str, char c);
void string_util_strcpy(char* buf, size_t buf_capacity, const char* str);
void string_util_strcat(char* buf, size_t buf_capacity, const char* str);
void string_util_replace(char *buf, size_t buf_capacity, const char *old_str, const char *new_str);
bool string_util_ends_with(const char* buf, const char* str);

#endif
