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
