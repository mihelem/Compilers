#ifndef dfazer_header
#define dfazer_header
/* Author: Michele Miccinesi 02022020 			*/
/* DFA-zer: from NFA to DFA and minimization 	*/

#include "commons.h"
#include "vector_macros.h"
#include "binary_heap_macros.h"
#include "trie.h"
#include "nfa.h"

static vector_type(pnfa_node_t) *make_unique (vector_type(pnfa_node_t) *nodes);
// @return: ptr to vector with unique ptrs to nfa nodes of the empty transition closure
vector_type(pnfa_node_t) *empty_trans_closure (vector_type(pnfa_node_t) *nodes);
heap_def_header(uint64_t, <, less);
// @return: ptr to vector of sorted ids in the subset
vector_type(uint64_t) *pnfa_nodes_to_ids (vector_type(pnfa_node_t) *in, vector_type(uint64_t) *out);

vector_type(pnfa_node_t) transformed_subset_construction_nfa(
	nfa_t *in, 
	nfa_t *out,
	vector_type(pnfa_node_t) *(*subset_transform) (nfa_t *, nfa_t *, vector_type(pnfa_node_t) *));
vector_type(pnfa_node_t) *empty_transform_subset_nfa(
	nfa_t *in, 
	nfa_t *out, 
	vector_type(pnfa_node_t) *subset);
vector_type(pnfa_node_t) *add_initial_transform_subset_nfa(
	nfa_t *in, 
	nfa_t *out, 
	vector_type(pnfa_node_t) *subset);
vector_type(pnfa_node_t) nfa_to_dfa(nfa_t *nfa, nfa_t *dfa);
vector_type(pnfa_node_t) nfa_to_search_automata(nfa_t *in, nfa_t *out);

vector_type(pnfa_node_t) reverse_nfa(nfa_t *_nfa, nfa_t *r_nfa);
vector_type(pnfa_node_t) brzozowski_minimization_of_nfa(nfa_t *nfa, nfa_t *mdfa);
nfa_t intersection_dfazing_nfa (nfa_t *nfa1, nfa_t *nfa2);

#endif