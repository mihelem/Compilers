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
	n |= n>>32;	\
	++n;		\
	n >>= 1;

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

#define HI_HEAP_MACRO(cmp, up, left, right, i) \
	if (left cmp right) { 	\
		S_HEAP_MACRO(cmp, left, up);	\
		i = L_HEAP_MACRO(i);		\
	} else {	\
		S_HEAP_MACRO(cmp, right, up);	\
		i = R_HEAP_MACRO(i);		\
	}

#define heap_def_header(type, cmp, cmp_name) \
type *heapify_##cmp_name##_##type ( const size_t n, type data[n]  );	\
type *heapify_new_top_##cmp_name##_##type ( const size_t n, type data[n] );	\
type *heap_sort_##cmp_name##_##type ( size_t n, type data[n] );	\

#define heap_def(type, cmp, cmp_name) \
type *heapify_##cmp_name##_##type ( const size_t n, type data[n] ) {	\
	if ( n<2 ) {	\
		return data;	\
	}	\
	--data;		\
	type temp;	\
		\
	size_t end_down = n;	\
	if ( (end_down & 1) == 0 ) {	\
		size_t m = (end_down >> 1);	\
		S_HEAP_MACRO(cmp, data[end_down], data[m]);	\
	}	\
	if (end_down <= 2) {	\
		return data;	\
	}	\
	size_t begin_down = end_down-1;	\
	F_HEAP_MACRO(begin_down);	\
	size_t begin_up = (begin_down>>1);	\
	\
	while (begin_down > 1) {	\
		for( size_t down = begin_down, up = begin_up; down < end_down; down += 2, 	++up ) {	\
			H_HEAP_MACRO(cmp,  data[up], data[down], data[down+1] );	\
		}	\
		end_down = begin_down;	\
		begin_down = begin_up;	\
		begin_up >>= 1;	\
	}	\
	\
	return data+1;	\
}	\
\
type *heapify_new_top_##cmp_name##_##type ( const size_t n, type data[n] ) {	\
	--data;	\
	type temp;	\
	\
	size_t up = 1, left = 2, right = 3;	\
	while( right <= n ) {	\
		HI_HEAP_MACRO(cmp, data[up], data[left], data[right], up);	\
		left = L_HEAP_MACRO(up);	\
		right = left|1;	\
	}	\
	if ( left <= n ) {	\
		S_HEAP_MACRO(cmp,  data[left], data[up] );	\
	}	\
	\
	return data+1;	\
}	\
\
type *heap_sort_##cmp_name##_##type ( size_t n, type data[n] ) {	\
	type temp;	\
	while ( n-->0 ) {	\
		swap( data[0], data[n], temp );	\
		heapify_new_top_##cmp_name##_##type ( n, data );		\
	}	\
	return data;	\
}

#define heapify(type, cmp_name, data, size) heapify_helper(type, cmp_name, data, size)
#define heapify_helper(type, cmp_name, data, size) heapify_parameter_expander(type, cmp_name, data, size)
#define heapify_parameter_expander(type, cmp_name, data, size) heapify_##cmp_name##_##type (size, data)

#define heapify_new_top(type, cmp_name, data, size) heapify_new_top_helper(type, cmp_name, data, size)
#define heapify_new_top_helper(type, cmp_name, data, size) heapify_new_top_parameter_expander(type, cmp_name, data, size)
#define heapify_new_top_parameter_expander(type, cmp_name, data, size) heapify_new_top_##cmp_name##_##type (size, data)

#define heap_sort(type, cmp_name, data, size) heap_sort_helper(type, cmp_name, data, size)
#define heap_sort_helper(type, cmp_name, data, size) heap_sort_parameter_expander(type, cmp_name, data, size)
#define heap_sort_parameter_expander(type, cmp_name, data, size) heap_sort_##cmp_name##_##type(size, data)

#endif