/* naive strings */
#include <stdio.h>
#include <stdlib.h>
#include "string_t.h"

string_t *place_string_t (string_t *str) {
	if (str) {
		str->begin = str->end = str->capacity = 0;
		str->data = NULL;
	}
	return str;
}

string_t *destroy_string_t (string_t *str) {
	if (str != NULL) {
		if (str->data) {
			free(str->data);
		}
		str->data = NULL;
		str->begin = str->end = str->capacity = 0;
	}
	return str;
}

string_t *expand_string_t (string_t *str, size_t n) {
	if (str == NULL) {
		return str;
	}
	size_t new_capacity = str->capacity;
	n += str->end;
	if (n > new_capacity) {
		do {
			new_capacity = 2*new_capacity+1;
		} while (n > new_capacity);
		str->data = realloc(str->data, new_capacity);
		str->capacity = new_capacity;
	}

	return str;
}

string_t *cat_bytes_string_t (string_t *str, const char *bytes, size_t n) {
	expand_string_t(str, n);
	memcpy(str->data+str->end, bytes, n);
	str->end += n;

	return str;
}

string_t *cat_cstring_string_t (string_t *str, const char *cstr) {
	if (cstr == NULL) {
		return str;
	}

	size_t n = 0;
	while (cstr[n]) {
		++n;
	}
	return cat_bytes_string_t(str, cstr, n);
}

string_t *cat_string_t (string_t *dest, const string_t *source) {
	if (source == NULL) {
		return dest;
	}
	return cat_bytes_string_t(dest, source->data+source->begin, source->end-source->begin);
}

string_t *push_back_string_t (string_t *str, char c) {
	expand_string_t(str, str->end+1);
	str->data[str->end++] = c;

	return str;
}

string_t *clear_string_t (string_t *str) {
	if (str == NULL) {
		return NULL;
	}
	str->begin = str->end = 0;
	return str;
}

char *cstringify_string_t (string_t *str) {
	if (str == NULL) {
		return NULL;
	}
	return push_back_string_t(str, 0)->data+str->begin;
}

