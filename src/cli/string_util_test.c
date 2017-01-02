#include "mechanic/string_util.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

static bool assert_strlen(size_t expected_length, const char* s, const char* file, const int line) {
	size_t len = strlen(s);
	bool valid = len == expected_length;
	if( !valid ) {
		fprintf(stderr, "TEST FAILED! (%s:%d) Expected string '%s' to be of length %zu, but is %zu.\n", file, line, s, expected_length, len);
	}
	return valid;
}

static void test_strcpy_copies_string_when_fits() {
	char buf[4];
	string_util_strcpy(buf, 4, "abc");
	assert_strlen(3, buf, __FILE__, __LINE__);
}

static void test_strcpy_limits_copied_string_when_doenst_fit() {
	char buf[4];
	string_util_strcpy(buf, 4, "abcd");
	assert_strlen(3, buf, __FILE__, __LINE__);
}

int main(const int argc, const char** argv) {
	test_strcpy_copies_string_when_fits();
	test_strcpy_limits_copied_string_when_doenst_fit();
}
