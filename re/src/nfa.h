#ifndef nfa_header
#define nfa_header
/* Author: Michele Miccinesi 02022020 	*/
/* NFA 									*/

#include "commons.h"
#include "vector_macros.h"
#include "range.h"

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

nfa_node_t *place_nfa_node_t(nfa_node_t *node, uint64_t flags);
nfa_node_t make_nfa_node_t(uint64_t flags);
void destroy_nfa_node_t (nfa_node_t* node);

typedef struct nfa_t {
	vector_pnfa_node_t_t initial, final;
} nfa_t;

nfa_t *place_nfa_t (nfa_t *nfa);
void destroy_nfa_t (nfa_t *nfa);
void copy_nfa_t (const nfa_t *source, nfa_t *dest);
void add_nfa_edge (nfa_node_t *source, nfa_node_t *dest, uint8_t literal);
void add_nfa_initial (nfa_t *nfa, nfa_node_t *initial);
void add_nfa_final (nfa_t *nfa, nfa_node_t *final);
void clear_nfa_initial (nfa_t *nfa);
void clear_nfa_final (nfa_t *nfa);
// '\0' is the label of the empty transistion
nfa_node_t *literal_to_nfa_helper (nfa_node_t *initial, nfa_node_t *final, uint8_t c);
nfa_t range_to_nfa (range_t *range);
nfa_t literal_to_nfa (uint8_t literal);
nfa_t *make_monoextremal_nfa (nfa_t *nfa);
nfa_t *make_avoidable_nfa (nfa_t *nfa);
nfa_t *make_repeatable_nfa (nfa_t *nfa);
nfa_t concatenation_nfa (const nfa_t *nfa_old1, const nfa_t *nfa_old2);
nfa_t union_nfa (const nfa_t *nfa1, const nfa_t *nfa2);

vector_pnfa_node_t_t dfs_with_action_nfa (const nfa_t *nfa, void (*) (nfa_node_t *));
void print_nfa_node (nfa_node_t *node);
void print_as_mermaid_body_nfa_node (nfa_node_t *node);
void dummy_print_nfa_node (nfa_node_t *);
void print_nfa (const nfa_t *nfa, void (*) (nfa_node_t *));
void print_nodes (vector_type(pnfa_node_t) *nodes);
vector_type(pnfa_node_t) get_copy_of_nodes (vector_type(pnfa_node_t) *node_ptrs);
vector_type(pnfa_node_t) get_copy_with_edges_of_nodes (vector_type(pnfa_node_t) *node_ptrs);
vector_type(pnfa_node_t) *unset_flags (vector_type(pnfa_node_t) *nodes, uint64_t flags);
vector_type(pnfa_node_t) *set_flags (vector_type(pnfa_node_t) *nodes, uint64_t flags);
vector_type(pnfa_node_t) setup_nodes_from_nfa (nfa_t *_nfa);
vector_type(pnfa_node_t) *setup_nfa_from_nodes (nfa_t *nfa, vector_type(pnfa_node_t) *nodes);
nfa_t *set_extremality_flags (nfa_t *nfa);

vector_def_header(nfa_t);

#endif