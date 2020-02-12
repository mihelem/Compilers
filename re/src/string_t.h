#ifndef string_t_header
#define string_t_header

/* naive string */
#include <string.h>


typedef struct string_t {
	char *data;
	size_t begin, end;
	size_t capacity;
} string_t;

string_t *place_string_t (string_t str[static 1]);
string_t *destroy_string_t (string_t str[static 1]);
string_t *expand_string_t (string_t str[static 1], size_t n);
string_t *cat_bytes_string_t (string_t str[static 1], size_t n, const char bytes[n]);
string_t *cat_cstring_string_t (string_t str[static 1], const char *cstr);
string_t *cat_string_t (string_t dest[static 1], const string_t source[static 1]);
string_t *push_back_string_t (string_t str[static 1], char c);
string_t *clear_string_t (string_t str[static 1]);
char *cstringify_string_t (string_t str[static 1]);

#endif