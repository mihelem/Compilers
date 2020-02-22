#ifndef range_header
#define range_header
/* Author: Michele Miccinesi 02022020 	*/
#include "commons.h"
#include "vector_macros.h"

/* RANGES - e.g. [a-z] &tc */
typedef struct range_t {
	uint64_t literals[4];
} range_t;

bool is_in_range (range_t range[static 1], uint8_t c);
void set_in_range (range_t range[static 1], uint8_t c);
range_t *complement_range (range_t range[static 1]);
range_t *clear_range (range_t range[static 1]);
range_t union_range (range_t range1[static 1], range_t range2[static 1]);

#endif