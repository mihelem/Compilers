#ifndef trie_header
#define trie_header
/* Author: Michele Miccinesi 02022020 	*/
/* Naive Trie for subsets matching 		*/

#include "binary_heap_macros.h"

typedef struct trie_node_t {
	uint64_t out[130];
} trie_node_t;

extern const trie_node_t empty_trie_node;
trie_node_t *place_trie_node_t (trie_node_t *ptrie_node);

vector_def_header(trie_node_t);
typedef vector_type(trie_node_t) trie_t;

trie_t trie(size_t capacity);

#define destroy_trie(_trie) destroy_vector(trie_node_t, _trie)

#define destroy_trie_node_t(...)
trie_t *add_empty_node_to_trie (trie_t *_trie);

vector_def_header(uint8_t);
typedef vector_type(uint8_t) bytes_t;

vector_def_header(uint64_t);

bytes_t *append_uint64_to_bytes (bytes_t *bytes, uint64_t x);
bytes_t *append_separator_to_bytes (bytes_t *bytes);
bytes_t *append_eol_to_bytes (bytes_t *bytes);
bytes_t *encode_subset_to_bytes (vector_type(uint64_t) *in, bytes_t *out);
// @return: pointer to id associated with eol
uint64_t *add_subset_to_trie (trie_t *_trie, bytes_t *bytes);

#endif