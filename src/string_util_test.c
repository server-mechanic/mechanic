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

#include "string_util.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool assert_strlen(size_t expected_length, const char* s, const char* file, const int line) {
	size_t len = strlen(s);
	bool valid = len == expected_length;
	if( !valid ) {
		fprintf(stderr, "TEST FAILED! (%s:%d) Expected string '%s' to be of length %zu, but is %zu.\n", file, line, s, expected_length, len);
		exit(1);
	}
	return valid;
}

static bool test_strcpy_copies_string_when_fits() {
	char buf[4];
	string_util_strcpy(buf, 4, "abc");
	return assert_strlen(3, buf, __FILE__, __LINE__);
}

static bool test_strcpy_limits_copied_string_when_doenst_fit() {
	char buf[4];
	string_util_strcpy(buf, 4, "abcd");
	return assert_strlen(3, buf, __FILE__, __LINE__);
}

int main(const int argc, const char** argv) {
	test_strcpy_copies_string_when_fits();
	test_strcpy_limits_copied_string_when_doenst_fit();
	return 0;
}
