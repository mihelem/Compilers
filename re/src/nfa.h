#ifndef nfa_header
#define nfa_header
/* Author: Michele Miccinesi 02022020 	*/
/* NFA 									*/

#include "commons.h"
#include "vector_macros.h"
#include "range.h"
#include "trie.h"

#define flag_nfa_node_visited 1
#define flag_nfa_node_initial 2
#define flag_nfa_node_final 4

typedef struct vector_pnfa_node_t vector_pnfa_node_t_t;

typedef struct nfa_node_t {
	uint64_t id;
	uint64_t flags;
	vector_pnfa_node_t_t *out; 
} nfa_node_t;
typedef nfa_node_t* pnfa_node_t;

#define destroy_pnfa_node_t(...)

vector_def_header(pnfa_node_t)

nfa_node_t *place_nfa_node_t(nfa_node_t *node, const uint64_t flags);
nfa_node_t make_nfa_node_t(const uint64_t flags);
void destroy_nfa_node_t (nfa_node_t node[static 1]);

typedef struct nfa_t {
	vector_pnfa_node_t_t initial, final;
} nfa_t;

nfa_t *place_nfa_t (nfa_t *nfa);
nfa_t *destroy_nodes_of_nfa (nfa_t nfa[static 1]);
void destroy_nfa_t (nfa_t nfa[static 1]);
void copy_nfa_t (const nfa_t source[static 1], nfa_t dest[static 1]);
void add_nfa_empty_transition (nfa_node_t source[static 1], nfa_node_t dest[static 1]);
void add_nfa_edge (nfa_node_t source[static 1], nfa_node_t dest[static 1], const size_t literal);
void add_nfa_initial (nfa_t nfa[static 1], nfa_node_t initial[static 1]);
void add_nfa_final (nfa_t nfa[static 1], nfa_node_t final[static 1]);
void clear_nfa_initial (nfa_t nfa[static 1]);
void clear_nfa_final (nfa_t nfa[static 1]);
// '\0' is the label of the empty transistion
nfa_t range_to_nfa (range_t range[static 1]);
nfa_t literal_to_nfa (const uint8_t literal);
nfa_t *make_monoextremal_nfa (nfa_t nfa[static 1]);
nfa_t *make_avoidable_nfa (nfa_t nfa[static 1]);
nfa_t *make_repeatable_nfa (nfa_t nfa[static 1]);
nfa_t concatenation_nfa (const nfa_t nfa_old1[static 1], const nfa_t nfa_old2[static 1]);
nfa_t *concatenate_nfa (nfa_t nfa[static 1], nfa_t nfa1[static 1], nfa_t nfa2[static 1]);
nfa_t union_nfa (const nfa_t nfa1[static 1], const nfa_t nfa2[static 1]);
nfa_t *unite_nfa (nfa_t nfa[static 1], nfa_t nfa1[static 1], nfa_t nfa2[static 1]);

vector_pnfa_node_t_t dfs_with_action_nfa (nfa_t nfa[static 1], void (nfa_node_t *));
void print_nfa_node (nfa_node_t node[static 1]);
void print_as_mermaid_body_nfa_node (nfa_node_t node[static 1]);
void dummy_print_nfa_node (nfa_node_t *_node);
void clean_flags_of_nfa_node (nfa_node_t node[static 1]);
void print_nfa (nfa_t nfa[static 1], void (nfa_node_t *));
vector_type(pnfa_node_t) get_copy_of_nodes (const vector_type(pnfa_node_t) node_ptrs[static 1]);
vector_type(pnfa_node_t) get_copy_with_edges_of_nodes (const vector_type(pnfa_node_t) node_ptrs[static 1]);
void print_nodes (vector_type(pnfa_node_t) *nodes);
vector_type(pnfa_node_t) setup_nodes_from_nfa (nfa_t nfa[static 1]);
vector_type(pnfa_node_t) *setup_nfa_from_nodes (nfa_t nfa[static 1], vector_type(pnfa_node_t) nodes[static 1]);
nfa_t *set_extremality_flags (nfa_t nfa[static 1]);
vector_type(pnfa_node_t) *unset_flags (vector_type(pnfa_node_t) nodes[static 1], const uint64_t flags);
vector_type(pnfa_node_t) *set_flags (vector_type(pnfa_node_t) nodes[static 1], const uint64_t flags);
nfa_t *apply_flags_to_nfa (nfa_t nfa[static 1], const uint64_t flags, const uint64_t unflags);
vector_type(pnfa_node_t) *make_unique_nfa_nodes (vector_type(pnfa_node_t) nodes[static 1]);

vector_def_header(nfa_t);

vector_def_header(vector_type(pnfa_node_t));
typedef vector_type(vector_type(pnfa_node_t)) subset_dfa_nodes_t;

nfa_t intersection_nfa (const nfa_t nfa1[static 1], const nfa_t nfa2[static 1]);

#endif