#ifndef binary_heap_macros
#define binary_heap_macros
/* Author: Michele Miccinesi 02022020 	*/
/* Simple BINARY HEAP 					*/

#include "vector_macros.h"

#define L_HEAP_MACRO(n) ((n)<<1)
#define R_HEAP_MACRO(n) (((n)<<1)|1)
#define F_HEAP_MACRO(n) \
	n |= n>>1;	\
	n |= n>>2;	\
	n |= n>>4;	\
	n |= n>>8;	\
	n |= n>>16;	\
	n |= n>>32;

#define swap(a, b, temp) \
	temp = (a);	\
	(a) = (b);	\
	(b) = temp;	

#define S_HEAP_MACRO(cmp, down, up)	\
	if (down cmp up) {	\
		swap(down, up, temp);	\
	}

#define H_HEAP_MACRO(cmp, up, left, right) \
	if (left cmp right) { 	\
		S_HEAP_MACRO(cmp, left, up);	\
	} else {	\
		S_HEAP_MACRO(cmp, right, up);	\
	}

#define heap_def_header(type, cmp, cmp_name) \
type *heapify_##cmp_name##_##type ( size_t n, type data[n] );	\
type *heap_update_##cmp_name##_##type ( size_t up, const size_t n, type data[n] );	\
type *heap_sort_##cmp_name##_##type ( size_t n, type data[n] );	\
size_t not_heap_##cmp_name##_##type ( size_t n, type data[n] );

#define heap_def(type, cmp, cmp_name) \
type *heapify_##cmp_name##_##type ( const size_t n, type data[n] ) {	\
	for ( size_t up = n>>1; up > 0; --up ) {	\
		heap_update_##cmp_name##_##type (up, n, data);	\
	}	\
	\
	return data;	\
}	\
\
type *heap_update_##cmp_name##_##type ( size_t up, const size_t n, type data[n] ) {	\
	--data;	\
	type temp;	\
	\
	size_t left = L_HEAP_MACRO(up);	\
	size_t right = left | 1;	\
	while( right <= n ) {	\
		if (data[left] cmp data[right]) {	\
			if (data[left] cmp data[up]) { 	\
				swap(data[left], data[up], temp);	\
				up = left;	\
			} else { \
				return data+1;	\
			}	\
		} else if (data[right] cmp data[up]) {	\
			swap(data[right], data[up], temp);	\
			up = right;	\
		} else {	\
			return data+1;	\
		}	\
		\
		left = L_HEAP_MACRO(up);	\
		right = left|1;	\
	}	\
	if ( left <= n ) {	\
		S_HEAP_MACRO(cmp, data[left], data[up]);	\
	}	\
	\
	return data+1;	\
}	\
\
type *heap_sort_##cmp_name##_##type ( size_t n, type data[n] ) {	\
	type temp;	\
	while ( n-->1 ) {	\
		swap( data[0], data[n], temp );	\
		heap_update_##cmp_name##_##type ( 1, n, data );		\
	}	\
	return data;	\
}	\
\
size_t not_heap_##cmp_name##_##type ( size_t n, type data[n] ) {	\
	--data;	\
	while ( n>1 ) {	\
		if ( data[n] cmp data[n>>1] ) {	\
			return n;	\
		}	\
		--n;	\
	}	\
	return 0;	\
}

#define heapify(type, cmp_name, size, data) heapify_helper(type, cmp_name, size, data)
#define heapify_helper(type, cmp_name, size, data) heapify_parameter_expander(type, cmp_name, size, data)
#define heapify_parameter_expander(type, cmp_name, size, data) heapify_##cmp_name##_##type (size, data)

#define heap_update(type, cmp_name, size, data) heap_update_helper(type, cmp_name, up, size, data)
#define heap_update_helper(type, cmp_name, up, size, data) heap_update_parameter_expander(type, cmp_name, up, size, data)
#define heap_update_parameter_expander(type, cmp_name, up, size, data) heap_update_##cmp_name##_##type (up, size, data)

#define heap_sort(type, cmp_name, size, data) heap_sort_helper(type, cmp_name, size, data)
#define heap_sort_helper(type, cmp_name, size, data) heap_sort_parameter_expander(type, cmp_name, size, data)
#define heap_sort_parameter_expander(type, cmp_name, size, data) heap_sort_##cmp_name##_##type(size, data)

#define not_heap(type, cmp_name, size, data) not_heap_helper(type, cmp_name, size, data)
#define not_heap_helper(type, cmp_name, size, data) not_heap_parameter_expander(type, cmp_name, size, data)
#define not_heap_parameter_expander(type, cmp_name, size, data) not_heap_##cmp_name##_##type(size, data)

#endif