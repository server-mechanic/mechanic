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

#ifndef __MECHANIC_LOG_H__
#define __MECHANIC_LOG_H__

#include <stdio.h>
#include <stdbool.h>

#define LOG_DEBUG(msg) log_debug(__FILE__, __LINE__, (msg))
#define LOG_DEBUG1(msg,arg0) log_debug(__FILE__, __LINE__, (msg), (arg0))
#define LOG_DEBUG2(msg,arg0,arg1) log_debug(__FILE__, __LINE__, (msg), (arg0), (arg1))

#define LOG_INFO(msg) log_info(__FILE__, __LINE__, (msg))
#define LOG_INFO1(msg,arg0) log_info(__FILE__, __LINE__, (msg), (arg0))
#define LOG_INFO2(msg,arg0,arg1) log_info(__FILE__, __LINE__, (msg), (arg0),(arg1))

#define LOG_WARN(msg) log_warn(__FILE__, __LINE__, (msg))
#define LOG_WARN1(msg,arg0) log_warn(__FILE__, __LINE__, (msg), (arg0))
#define LOG_WARN2(msg,arg0,arg1) log_warn(__FILE__, __LINE__, (msg), (arg0), (arg1))

#define LOG_ERROR(msg) log_error(__FILE__, __LINE__, (msg))
#define LOG_ERROR1(msg,arg0) log_error(__FILE__, __LINE__, (msg), (arg0))
#define LOG_ERROR2(msg,arg0,arg1) log_error(__FILE__, __LINE__, (msg), (arg0), (arg1))

void log_set_output_stream(FILE* out);
void log_set_log_level(bool verbose);
void log_debug(const char* file, int line, const char* format, ...);
void log_info(const char* file, int line, const char* format, ...);
void log_warn(const char* file, int line, const char* format, ...);
void log_error(const char* file, int line, const char* format, ...);

#endif
