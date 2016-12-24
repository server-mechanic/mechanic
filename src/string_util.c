#include <string.h>
#include <stdlib.h>
#include "mechanic/string_util.h"

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
