#ifndef range_header
#define range_header
/* Author: Michele Miccinesi 02022020 	*/
#include "commons.h"
#include "vector_macros.h"

/* RANGES - e.g. [a-z] &tc */
typedef struct range_t {
	uint64_t literals[4];
} range_t;

bool is_in_range (range_t *range, uint8_t c);
bool is_complemented_range (range_t *range);
void set_in_range (range_t *range, uint8_t c);
range_t *set_complemented_range (range_t *range);
range_t *complement_range (range_t *range);
range_t *clear_range (range_t *range);
range_t union_range (range_t *range1, range_t *range2);
size_t range_to_cstring (range_t *range, uint8_t *s);

#endif