/* naive strings */
#include <stdio.h>
#include <stdlib.h>
#include "string_t.h"

string_t *place_string_t (string_t str[static 1]) {
	str->begin = str->end = str->capacity = 0;
	str->data = NULL;
	
	return str;
}

string_t *destroy_string_t (string_t str[static 1]) {
	if (str->data) {
		free(str->data);
	}
	str->data = NULL;
	str->begin = str->end = str->capacity = 0;
	
	return str;
}

string_t *expand_string_t (string_t str[static 1], size_t n) {
	size_t new_capacity = str->capacity;
	n += str->end;
	if (n > new_capacity) {
		do {
			new_capacity = 2*new_capacity+1;
		} while (n > new_capacity);
		char *data = realloc(str->data, new_capacity);
		if (!data) {
			perror("OUT OF MEMORY\n");
			exit(EXIT_FAILURE);
		} else {
			str->data = data;
			str->capacity = new_capacity;
		}
	}

	return str;
}

string_t *cat_bytes_string_t (string_t str[static 1], size_t n, const char bytes[n]) {
	expand_string_t(str, n);
	memcpy(str->data+str->end, bytes, n);
	str->end += n;

	return str;
}

string_t *cat_cstring_string_t (string_t str[static 1], const char *cstr) {
	if (cstr == NULL) {
		return str;
	}

	size_t n = 0;
	while (cstr[n]) {
		++n;
	}
	return cat_bytes_string_t(str, n, cstr);
}

string_t *cat_string_t (string_t dest[static 1], const string_t source[static 1]) {
	return cat_bytes_string_t(dest, source->end-source->begin, source->data+source->begin);
}

string_t *push_back_string_t (string_t str[static 1], char c) {
	expand_string_t(str, str->end+1);
	str->data[str->end++] = c;

	return str;
}

string_t *clear_string_t (string_t str[static 1]) {
	str->begin = str->end = 0;
	return str;
}

char *cstringify_string_t (string_t str[static 1]) {
	return push_back_string_t(str, 0)->data+str->begin;
}

