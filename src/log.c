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

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>

static FILE* out = NULL;
static bool verbose = false;

#define MESSAGE_BUF_LENGTH 4000

void log_set_output_stream(FILE* p_out) {
	out = p_out;
}

static void write_log(const char* file, int line, const char* level, const char* format, va_list arg_list) {
	time_t rawtime;
	struct tm * timeinfo;
	struct timeval tv;
	int millisec;
	pid_t thread_id;

	thread_id = syscall(__NR_gettid);

	FILE* l_out = out != NULL ? out : stderr;
	char message_buf[MESSAGE_BUF_LENGTH];
	vsnprintf(message_buf, MESSAGE_BUF_LENGTH, format, arg_list);

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	gettimeofday(&tv, NULL);
	millisec = lrint(tv.tv_usec/1000.0);
	if (millisec>=1000) { // Allow for rounding up to nearest second
		millisec -=1000;
		tv.tv_sec++;
	}

	fprintf(l_out, "[%04d-%02d-%02d %02d:%02d:%02d.%03d] %s %d (%s:%d) %s\n", timeinfo->tm_year+1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, millisec, level, thread_id, file, line, message_buf);
}

void log_set_log_level(bool p_verbose) {
	verbose = p_verbose;
}

void log_debug(const char* file, int line, const char* format, ...) {
	va_list arg_list;

	if( verbose ) {
		va_start(arg_list, format);
		write_log(file, line, "DEBUG", format, arg_list); 
		va_end(arg_list);
	}
}

void log_info(const char* file, int line, const char* format, ...) {
	va_list arg_list;
	va_start(arg_list, format);
	write_log(file, line, "INFO ", format, arg_list); 
	va_end(arg_list);
}

void log_error(const char* file, int line, const char* format, ...) {
	va_list arg_list;
	va_start(arg_list, format);
	write_log(file, line, "ERROR", format, arg_list); 
	va_end(arg_list);
}

