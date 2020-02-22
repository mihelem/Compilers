#ifndef vector_macros
#define vector_macros
/* Author: Michele Miccinesi 02022020 	*/
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "commons.h"

#define CAT(a, b) CAT_HELPER(a, b)
#define CAT_HELPER(a, b) a ## b

#define vector_name(type) CAT(vector_, type)
#define vector_type(type) CAT(vector_name(type), _t)

#define vector_def_header(type) vector_def_header_helper(type, vector_name(type), vector_type(type))
#define vector_def_header_helper(type, name, vectype) vector_def_header_param_expander(type, name, vectype)
#define vector_def_header_param_expander(type, _vector_name, _vector_type) \
typedef struct _vector_name { \
	type *data;	\
	size_t begin, end; \
	size_t capacity; \
} _vector_type; \
\
_vector_type new_##_vector_type (const size_t capacity);	\
bool is_empty_##_vector_type (const _vector_type vec[static 1]);		\
_vector_type *clear_##_vector_type (_vector_type vec[static 1]);		\
_vector_type *destroy_##_vector_type (_vector_type vec[static 1]);	\
_vector_type *expand_back_##_vector_type (_vector_type vec[static 1]);	\
_vector_type *push_back_##_vector_type (_vector_type vec[static 1], type const el);	\
_vector_type *concatenate_##_vector_type (_vector_type vec[static 1], const _vector_type vec1[static 1]);	\
_vector_type *copy_##_vector_type (_vector_type const source[static 1], _vector_type dest[static 1]);		\
type pop_back_##_vector_type (_vector_type vec[static 1]);		\
type *get_##_vector_type (_vector_type vec[static 1], const size_t i);	\
type *back_##_vector_type (_vector_type vec[static 1]);	\
void set_##_vector_type (_vector_type vec[static 1], const size_t i, type el);	

#define vector_def(type) vector_def_helper(type, vector_name(type), vector_type(type))
#define vector_def_helper(type, name, vectype) vector_def_param_expander(type, name, vectype)
#define vector_def_param_expander(type, _vector_name, _vector_type) \
_vector_type new_##_vector_type (const size_t capacity) { \
	_vector_type vec;	\
	if (capacity) {	\
		vec.data = malloc(sizeof(type)*capacity);	\
		if (!vec.data) {	\
			perror("MEMORY SHORTAGE\n");	\
			exit(EXIT_FAILURE);	\
		}	\
	} else {	\
		vec.data = NULL;	\
	}	\
	vec.capacity = capacity;	\
	vec.begin = vec.end = 0;	\
	return vec; \
}	\
\
bool is_empty_##_vector_type (const _vector_type vec[static 1]) {	\
	return vec->begin == vec->end;	\
}	\
\
_vector_type *clear_##_vector_type (_vector_type vec[static 1]) {	\
	for( size_t i=vec->begin; i<vec->end; ++i ) {	\
		destroy_##type (vec->data+i);	\
	}	\
	vec->begin = vec->end = 0;	\
	return vec;	\
}	\
\
_vector_type *destroy_##_vector_type (_vector_type vec[static 1]) { \
	clear_##_vector_type(vec);	\
	vec->capacity = 0;	\
	free(vec->data);	\
	vec->data = NULL;	\
	\
	return vec;	\
}	\
\
_vector_type *expand_back_##_vector_type (_vector_type vec[static 1]) {	\
	if (vec->end == vec->capacity) {	\
		size_t capacity = 2*vec->capacity+1;	\
		type *data = realloc(vec->data, sizeof(type)*capacity);	\
		if (!data) {	\
			perror("MEMORY SHORTAGE\n");	\
			exit(EXIT_FAILURE);	\
		} else {	\
			vec->capacity = 2*vec->capacity+1;	\
			vec->data = data; 	\
		}	\
	}	\
	return vec;	\
}	\
\
_vector_type *push_back_##_vector_type (_vector_type vec[static 1], type const el) {	\
	expand_back_##_vector_type(vec);	\
	vec->data[vec->end++] = el;	\
	return vec;	\
}	\
\
_vector_type *concatenate_##_vector_type (_vector_type vec[static 1], const _vector_type vec1[static 1]) {	\
	for( size_t i = vec1->begin; i<vec1->end; ++i ) {	\
		push_back_##_vector_type (vec, vec1->data[i]);	\
	}	\
\
	return vec;	\
}	\
\
_vector_type *copy_##_vector_type (const _vector_type source[static 1], _vector_type dest[static 1]) { \
	return concatenate_##_vector_type (clear_##_vector_type (dest), source);	\
}	\
\
type pop_back_##_vector_type (_vector_type vec[static 1]) {	\
	type return_value = vec->data[--vec->end];	\
	destroy_##type(vec->data+vec->end);	\
	return return_value;	\
}	\
\
type *get_##_vector_type (_vector_type vec[static 1], const size_t i) { \
	return vec->data+i;	\
}	\
\
void set_##_vector_type (_vector_type vec[static 1], const size_t i, type el) { \
	vec->data[i] = el;	\
}	\
type *back_##_vector_type (_vector_type vec[static 1]) {\
	return vec->begin == vec->end 	\
		? NULL	\
		: vec->data+vec->end-1;	\
}


#define vector(type, capacity) make_vector(vector_type(type), capacity)
#define make_vector(name, capacity) make_vector_param_expander(name, capacity)
#define make_vector_param_expander(name, capacity) new_##name (capacity)

#define destroy_vector(type, vec) destroy_vector_helper(vector_type(type), vec)
#define destroy_vector_helper(name, vec) destroy_vector_param_expander(name, vec)
#define destroy_vector_param_expander(name, vec) destroy_##name (vec)

#define clear_vector(type, vec) clear_vector_helper(vector_type(type), vec)
#define clear_vector_helper(name, vec) clear_vector_param_expander(name, vec)
#define clear_vector_param_expander(name, vec) clear_##name (vec)

#define is_empty_vector(type, vec) is_empty_vector_helper(vector_type(type), vec)
#define is_empty_vector_helper(name, vec) is_empty_vector_param_expander(name, vec)
#define is_empty_vector_param_expander(name, vec) is_empty_##name (vec)

#define set_vector(type, vec, i, el) set_vector_helper(vector_type(type), vec, i, el)
#define set_vector_helper(name, vec, i, el) set_vector_param_expander(name, vec, i, el)
#define set_vector_param_expander(name, vec, i, el) set_##name(vec, i, el)

#define get_vector(type, vec, i) get_vector_helper(vector_type(type), vec, i)
#define get_vector_helper(name, vec, i) get_vector_param_expander(name, vec, i)
#define get_vector_param_expander(name, vec, i) get_##name (vec, i)

#define copy_vector(type, source, dest) copy_vector_helper(vector_type(type), source, dest)
#define copy_vector_helper(name, source, dest) copy_vector_param_expander(name, source, dest)
#define copy_vector_param_expander(name, source, dest) copy_##name (source, dest)

#define pop_back_vector(type, vec) pop_back_vector_helper(vector_type(type), vec)
#define pop_back_vector_helper(name, vec) pop_back_vector_param_expander(name, vec)
#define pop_back_vector_param_expander(name, vec) pop_back_##name (vec)

#define expand_back_vector(type, vec) expand_back_vector_helper(vector_type(type), vec)
#define expand_back_vector_helper(name, vec) expand_back_vector_parameter_expander(name, vec)
#define expand_back_vector_parameter_expander(name, vec) expand_back_##name(vec)

#define push_back_vector(type, vec, el) push_back_vector_helper(vector_type(type), vec, el)
#define push_back_vector_helper(name, vec, el) push_back_vector_param_expander(name, vec, el)
#define push_back_vector_param_expander(name, vec, el) push_back_##name(vec, el)

#define concatenate_vector(type, vec, vec1) concatenate_vector_helper(vector_type(type), vec, vec1)
#define concatenate_vector_helper(name, vec, vec1) concatenate_vector_param_expander(name, vec, vec1)
#define concatenate_vector_param_expander(name, vec, vec1) concatenate_##name(vec, vec1)

#define back_vector(type, vec) back_vector_helper(vector_type(type), vec)
#define back_vector_helper(name, vec) back_vector_param_expander(name, vec)
#define back_vector_param_expander(name, vec) back_##name (vec)

#define destroy_int(...)
#define destroy_int8_t(...)
#define destroy_uint8_t(...)
#define destroy_int16_t(...)
#define destroy_uint16_t(...)
#define destroy_int32_t(...)
#define destroy_uint32_t(...)
#define destroy_int64_t(...)
#define destroy_uint64_t(...)
#define destroy_size_t(...)
#define destroy(type) destroy_helper(type)
#define destroy_helper(type) destroy_##type

#endif