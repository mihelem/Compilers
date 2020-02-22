/* Author: Michele Miccinesi 02022020 	*/
/* Naive Trie for subsets matching 		*/

#include "trie.h"

trie_node_t *place_trie_node_t (trie_node_t *ptrie_node) {
	if (!ptrie_node) {
		perror("place_trie_node_t on NULL\n");
		exit(EXIT_FAILURE);
	}
	memset(ptrie_node->out, 255, sizeof(uint64_t)*130);

	return ptrie_node;
}

vector_def(trie_node_t);

trie_t trie(size_t capacity) {
	trie_t _trie = vector(trie_node_t, capacity);
	
	return _trie;
}

trie_t *add_empty_node_to_trie (trie_t _trie[static 1]) {
	expand_back_vector(trie_node_t, _trie);
	place_trie_node_t(_trie->data+_trie->end++);
	return _trie;
}

vector_def(uint8_t);
vector_def(uint64_t);

bytes_t *append_uint64_to_bytes (bytes_t bytes[static 1], uint64_t x) {
	do {
		push_back_vector(uint8_t, bytes, (uint8_t)(x & 127));
		x >>= 7;
	} while(x);

	return bytes;
}

bytes_t *append_separator_to_bytes (bytes_t bytes[static 1]) {
	push_back_vector(uint8_t, bytes, (uint8_t)128);

	return bytes;
}

bytes_t *append_eol_to_bytes (bytes_t bytes[static 1]) {
	push_back_vector(uint8_t, bytes, (uint8_t)129);

	return bytes;
}

bytes_t *encode_subset_to_bytes (vector_type(uint64_t) in[static 1], bytes_t out[static 1]) {
	if (in->begin < in->end) {
		for ( size_t i = in->begin; i < in->end-1; ++i ) {
			append_uint64_to_bytes(out, in->data[i]);
			append_separator_to_bytes(out);
		}
		append_uint64_to_bytes(out, in->data[in->end-1]);
	}
	append_eol_to_bytes(out);

	return out;
}

// @return: pointer to id associated with eol, stored in out[129]
uint64_t *add_subset_to_trie (trie_t _trie[static 1], bytes_t bytes[static 1]) {
	size_t i_trie = _trie->begin;
	size_t i_bytes = 0;
	uint64_t *pindex;
	while (bytes->data[i_bytes] != 129) {
		pindex = _trie->data[i_trie].out+bytes->data[i_bytes];
		if (*pindex == ~0) {
			*pindex = i_trie = _trie->end;
			add_empty_node_to_trie(_trie);
		} else {
			i_trie = *pindex;
		}
		++i_bytes;
	}
	return _trie->data[i_trie].out+129;
}

