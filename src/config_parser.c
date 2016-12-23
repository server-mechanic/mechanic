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
#include <ctype.h>
#include <string.h>
#include "mechanic/app_error.h"
#include "mechanic/config.h"
#include "mechanic/constants.h"
#include "mechanic/string_util.h"

typedef enum { INITIAL, IN_KEY, POST_KEY, EQ_SEEN, IN_VALUE, POST_VALUE, IN_COMMENT, DONE, ERROR } state_t;

static void clear_buffer(char* buffer) {
	buffer[0] = 0;
}

static void append_char(char* buffer, size_t buf_cap, char c) {
	char tmp[2] = {c, 0};
	string_util_strcat(buffer, buf_cap, tmp);
}

void config_parse(config_t* config, FILE* in, config_key_value_func_t key_value_handler, app_error_t* app_error) {
	char key_buffer[PATH_MAX_LENGTH] = "";
	char value_buffer[PATH_MAX_LENGTH] = "";

	state_t state = INITIAL;
	int c;

	while( state != DONE && (c=getc(in)) != 0 ) {
		switch( state ) {
			case INITIAL:
				if( c == EOF ) {
					state = DONE;
			}
				else if( '=' == c ) {
					state = ERROR;
				} else if( isspace(c) ) {
					/* skip */
				} else {
					state = IN_KEY;
					append_char(key_buffer, PATH_MAX_LENGTH, (char)c);
				}
				break;
			case IN_KEY:
				if( c == EOF ) {
					state = ERROR;
				}
				else if( '=' == c ) {
					state = EQ_SEEN;
				} else if( isspace(c) ) {
					state = POST_KEY;
				} else {
					append_char(key_buffer, PATH_MAX_LENGTH, (char)c);
				}
				break;
			case POST_KEY:
				if( c == EOF ) {
					state = ERROR;
				}
				else if( '=' == c ) {
					state = EQ_SEEN;
				} else if( isspace(c) ) {
					state = POST_KEY;
				} else {
					state = ERROR;
				}
				break;
			case EQ_SEEN:
				if( c == EOF ) {
					state = ERROR;
				} else if( isspace(c) ) {
					; /* skip */
				} else {
					append_char(value_buffer, PATH_MAX_LENGTH, (char)c);
					state = IN_VALUE;
				}
				break;
			case IN_VALUE:
				if( c == EOF ) {
					state = DONE;
				} else if ( c == '\n' ) {
					(*key_value_handler)(config, key_buffer, value_buffer, app_error);
					clear_buffer(key_buffer);
					clear_buffer(value_buffer);
					state = INITIAL;
				} else {
					append_char(value_buffer, PATH_MAX_LENGTH, (char)c);
					state = IN_VALUE;
				}
				break;
			case DONE:
				break;
			case ERROR:
				app_error_set(app_error, APP_ERROR_CONFIG_ERROR, __FILE__, __LINE__, "Parsing config failed.");
				return;
			default:
				break;
		}
	}

	return;
}

