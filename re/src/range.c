/* Author: Michele Miccinesi 02022020 	*/
#include "range.h"

bool is_in_range (range_t *range, uint8_t c) {
	return range->literals[c>>6] & (uint64_1 << (c & mask6));
}

bool is_complemented_range (range_t *range) {
	return is_in_range(range, 0);
}

void set_in_range (range_t *range, uint8_t c) {
	range->literals[c>>6] |= (uint64_1 << (c & mask6));
}

range_t *set_complemented_range (range_t *range) {
	set_in_range(range, 0);

	return range;
}

range_t *complement_range (range_t *range) {
	if (is_complemented_range(range)) {
		range->literals[0] = ~range->literals[0];
		range->literals[1] = ~range->literals[1];
		range->literals[2] = ~range->literals[2];
		range->literals[3] = ~range->literals[3];
	}

	return range;
}

range_t *clear_range (range_t *range) {
	range->literals[0] = range->literals[1] = range->literals[2] = range->literals[3] = 0;
	return range;
}

range_t union_range (range_t *range1, range_t *range2) {
	range_t range;
	clear_range(&range);
	range.literals[0] = range1->literals[0] | range2->literals[0];
	range.literals[1] = range1->literals[1] | range2->literals[1];
	range.literals[2] = range1->literals[2] | range2->literals[2];
	range.literals[3] = range1->literals[3] | range2->literals[3];

	return range;
}

size_t range_to_cstring (range_t *range, uint8_t *s) {
	complement_range(range);
	size_t j=0;
	for (uint8_t i=0; i++<255; ) {
		if (is_in_range(range, i)) {
			s[j++] = i;
		}
	}
	s[j] = 0;
	return j;
}
