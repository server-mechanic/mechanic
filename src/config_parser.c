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


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "app_error.h"
#include "config.h"
#include "string_util.h"

typedef enum { INITIAL, SECTION_BRACKET_LEFT_SEEN, IN_SECTION_NAME, SECTION_NAME_SEEN, IN_KEY, POST_KEY, EQ_SEEN, IN_VALUE, POST_VALUE, IN_COMMENT, DONE, ERROR } state_t;

static void clear_buffer(char* buffer) {
	buffer[0] = 0;
}

static void append_char(char* buffer, size_t buf_cap, char c) {
	char tmp[2] = {c, 0};
	string_util_strcat(buffer, buf_cap, tmp);
}

void config_parse(config_t* config, FILE* in, config_key_value_func_t key_value_handler, app_error_t* app_error) {
	char key_buffer[CONFIG_MAX_KEY_LENGTH] = "";
	char value_buffer[CONFIG_MAX_VALUE_LENGTH] = "";
	char section_buffer[CONFIG_MAX_SECTION_LENGTH] = "";

	state_t state = INITIAL;
	int c;

	while( state != DONE && (c=getc(in)) != 0 ) {
		switch( state ) {
			case INITIAL:
				if( c == EOF ) {
					state = DONE;
				} else if( '[' == c ) {
					clear_buffer(section_buffer);
					state = SECTION_BRACKET_LEFT_SEEN;
				} else if( '=' == c ) {
					state = ERROR;
				} else if( isspace(c) ) {
					/* skip */
				} else if( '#' == c ) {
					state = IN_COMMENT;
				} else {
					state = IN_KEY;
					append_char(key_buffer, CONFIG_MAX_KEY_LENGTH, (char)c);
				}
				break;
			case SECTION_BRACKET_LEFT_SEEN:
				if( c == EOF ) {
					state = ERROR;
				} else if ( ']' == c ) {
					state = INITIAL;
				} else if( isspace(c) ) {
					/* skip */
				} else {
					state = IN_SECTION_NAME;
					append_char(section_buffer, CONFIG_MAX_SECTION_LENGTH, (char)c);
				}
				break;
			case IN_SECTION_NAME:
				if( c == EOF ) {
					state = ERROR;
				} else if ( ']' == c ) {
					state = INITIAL;
				} else if( isspace(c) ) {
					state = SECTION_NAME_SEEN;
				} else {
					append_char(section_buffer, CONFIG_MAX_SECTION_LENGTH, (char)c);
				}
				break;
			case SECTION_NAME_SEEN:
				if( c == EOF ) {
					state = ERROR;
				} else if ( ']' == c ) {
					state = INITIAL;
				} else if( isspace(c) ) {
					/* skip */
				} else {
					state = ERROR;
				}
				break;
			case IN_KEY:
				if( c == EOF ) {
					state = ERROR;
				} else if( '=' == c ) {
					state = EQ_SEEN;
				} else if( isspace(c) ) {
					state = POST_KEY;
				} else {
					append_char(key_buffer, CONFIG_MAX_KEY_LENGTH, (char)c);
				}
				break;
			case POST_KEY:
				if( c == EOF ) {
					state = ERROR;
				} else if( '=' == c ) {
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
					append_char(value_buffer, CONFIG_MAX_VALUE_LENGTH, (char)c);
					state = IN_VALUE;
				}
				break;
			case IN_VALUE:
				if( c == EOF ) {
					state = DONE;
				} else if ( c == '#' ) {
					(*key_value_handler)(config, section_buffer, key_buffer, value_buffer, app_error);
					clear_buffer(key_buffer);
					clear_buffer(value_buffer);
					state = IN_COMMENT;
				} else if ( c == '\n' ) {
					(*key_value_handler)(config, section_buffer, key_buffer, value_buffer, app_error);
					clear_buffer(key_buffer);
					clear_buffer(value_buffer);
					state = INITIAL;
				} else {
					append_char(value_buffer, CONFIG_MAX_VALUE_LENGTH, (char)c);
					state = IN_VALUE;
				}
				break;
			case IN_COMMENT:
				if( c == EOF ) {
					state = DONE;
				} else if( c == '\n' ) {
					state = INITIAL;
				} else {
					; /* skip */
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

