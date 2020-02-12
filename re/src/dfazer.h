#ifndef dfazer_header
#define dfazer_header
/* Author: Michele Miccinesi 02022020 			*/
/* DFA-zer: from NFA to DFA and minimization 	*/

#include "commons.h"
#include "vector_macros.h"
#include "binary_heap_macros.h"
#include "trie.h"
#include "nfa.h"

// @return: ptr to vector with unique ptrs to nfa nodes of the empty transition closure
vector_type(pnfa_node_t) *empty_trans_closure (vector_type(pnfa_node_t) nodes[static 1]);
heap_def_header(uint64_t, <, less);
// @return: ptr to vector of sorted ids in the subset
vector_type(uint64_t) *pnfa_nodes_to_ids (const vector_type(pnfa_node_t) in[static 1], vector_type(uint64_t) out[static 1]);
vector_type(pnfa_node_t) *add_initial_transform_subset_nfa(
	nfa_t in[static 1], 
	nfa_t out[static 1], 
	vector_type(pnfa_node_t) subset[static 1]);

vector_type(pnfa_node_t) *empty_transform_subset_nfa(
	nfa_t in[static 1], 
	nfa_t out[static 1], 
	vector_type(pnfa_node_t) subset[static 1]);
vector_type(pnfa_node_t) nfa_to_dfa(nfa_t nfa[static 1], nfa_t dfa[static 1]);
vector_type(pnfa_node_t) nfa_to_search_automata(nfa_t in[static 1], nfa_t out[static 1]);
vector_type(pnfa_node_t) transformed_subset_construction_nfa(
	nfa_t in[static 1] , 
	nfa_t out[static 1] ,
	vector_type(pnfa_node_t) *subset_transform (nfa_t [static 1], nfa_t [static 1], vector_type(pnfa_node_t) [static 1]));

vector_type(pnfa_node_t) reverse_nfa(nfa_t nfa[static 1], nfa_t r_nfa[static 1]);
vector_type(pnfa_node_t) brzozowski_minimization_of_nfa(nfa_t nfa[static 1] , nfa_t mdfa[static 1] );
nfa_t intersection_dfazing_nfa (nfa_t nfa1[static 1], nfa_t nfa2[static 1]);

#endif