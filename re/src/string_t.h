#ifndef string_t_header
#define string_t_header

/* naive string */
#include <string.h>


typedef struct string_t {
	char *data;
	size_t begin, end;
	size_t capacity;
} string_t;

string_t *place_string_t (string_t *str);
string_t *destroy_string_t (string_t *str);
string_t *expand_string_t (string_t *str, size_t n);
string_t *cat_bytes_string_t (string_t *str, const char *bytes, size_t n);
string_t *cat_cstring_string_t (string_t *str, const char *cstr);
string_t *cat_string_t (string_t *dest, const string_t *source);
string_t *push_back_string_t (string_t *str, char c);
string_t *clear_string_t (string_t *str);
char *cstringify_string_t (string_t *str);

#endif