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


#include <string.h>
#include <stdlib.h>
#include "string_util.h"

void string_util_substring(char* buf, size_t buf_capacity, const char* str, char c) {
	const char* next_substr = strchr(str, c);
	if( next_substr == NULL ) {
		string_util_strcpy(buf, buf_capacity, str);
	} else {
		size_t substr_len = (size_t)(next_substr-str);
		strncpy(buf, str, substr_len);
		buf[substr_len] = '\0'; 
	}
}

void string_util_strcpy(char* buf, size_t buf_capacity, const char* str) {
	strncpy(buf, str, buf_capacity - 1);
	if (buf_capacity > 0) {
		buf[buf_capacity-1]= '\0';
	}
}

void string_util_strcat(char* buf, size_t buf_capacity, const char* str) {
	size_t buf_len = strlen(buf);

	strncat(&buf[buf_len], str, buf_capacity-buf_len-1);
	if (buf_capacity > 0) {
		buf[buf_capacity-1]= '\0';
	}
}

bool string_util_ends_with(const char* buf, const char* str) {
	size_t buf_len = strlen(buf);
	size_t str_len = strlen(str);

	if( buf_len < str_len ) {
		return false;
	}

	return strcmp(&(buf[buf_len-str_len-1]), str) == 0;
}

void string_util_replace(char *Str, size_t buf_cap, const char *OldStr, const char *NewStr)
{
	// TODO check buf_cap
	size_t OldLen, NewLen;
	char *p, *q;

	while(NULL != (p = strstr(Str, OldStr))) {
		OldLen = strlen(OldStr);
		NewLen = strlen(NewStr);
		memmove(q = p+NewLen, p+OldLen, strlen(p+OldLen)+1);
		memcpy(p, NewStr, NewLen);
		p = p + (NewLen-OldLen);
	}
}
