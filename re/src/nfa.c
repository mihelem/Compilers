/* Author: Michele Miccinesi 02022020 	*/
#include "nfa.h"

vector_def(pnfa_node_t)

nfa_node_t *place_nfa_node_t(nfa_node_t *node, uint64_t flags) {
	node->id = 0;
	node->flags = flags;
	node->out = calloc(256, sizeof(vector_pnfa_node_t_t));

	return node;
}

nfa_node_t make_nfa_node_t(uint64_t flags) {
	nfa_node_t node;
	place_nfa_node_t(&node, flags);

	return node;
}

void destroy_nfa_node_t (nfa_node_t* node) {
	if (node && node->out) {
		for( int i=0; i<256; ++i ) {
			destroy_vector(pnfa_node_t, node->out+i);
		}
		free(node->out);
		node->out = NULL;
	}
}

nfa_t *place_nfa_t (nfa_t *nfa) {
	nfa->initial = vector(pnfa_node_t, 0);
	nfa->final = vector(pnfa_node_t, 0);

	return nfa;
}

void destroy_nfa_t (nfa_t *nfa) {
	if (nfa) {
		destroy_vector(pnfa_node_t, &nfa->initial);
		destroy_vector(pnfa_node_t, &nfa->final);
	}
}

void copy_nfa_t (const nfa_t *source, nfa_t *dest) {
	if (source && dest) {
		copy_vector(pnfa_node_t, &source->initial, &dest->initial);
		copy_vector(pnfa_node_t, &source->final, &dest->final);
	}
}

void add_nfa_edge (nfa_node_t *source, nfa_node_t *dest, uint8_t literal) {
	push_back_vector(pnfa_node_t, source->out+literal, dest);
}

void add_nfa_initial (nfa_t *nfa, nfa_node_t *initial) {
	push_back_vector(pnfa_node_t, &nfa->initial, initial);
}

void add_nfa_final (nfa_t *nfa, nfa_node_t *final) {
	push_back_vector(pnfa_node_t, &nfa->final, final);
}

void clear_nfa_initial (nfa_t *nfa) {
	clear_vector(pnfa_node_t, &nfa->initial);
}

void clear_nfa_final (nfa_t *nfa) {
	clear_vector(pnfa_node_t, &nfa->final);
}
// '\0' is the label of the empty transistion
nfa_node_t *literal_to_nfa_helper (nfa_node_t *initial, nfa_node_t *final, uint8_t c) {
	nfa_node_t *node = place_nfa_node_t(malloc(sizeof(nfa_node_t)), 0);
	add_nfa_edge(initial, node, c);
	add_nfa_edge(node, final, 0);

	return node;
}

nfa_t range_to_nfa (range_t *range) {
	nfa_t nfa;
	place_nfa_t(&nfa);

	uint8_t s[256];
	size_t n = range_to_cstring(range, s);	
	
	nfa_node_t *initial = place_nfa_node_t(malloc(sizeof(nfa_node_t)), 0);
	nfa_node_t *final = place_nfa_node_t(malloc(sizeof(nfa_node_t)), 0);

	while (n--) {
		literal_to_nfa_helper(initial, final, s[n]);
	}

	add_nfa_initial(&nfa, initial);
	add_nfa_final(&nfa, final);


	vector_type(pnfa_node_t) nodes = setup_nodes_from_nfa(&nfa);
	destroy_vector(pnfa_node_t, &nodes); 
	return nfa;
}

nfa_t literal_to_nfa (uint8_t literal) {
	nfa_t nfa;
	place_nfa_t(&nfa);

	nfa_node_t *initial = place_nfa_node_t(malloc(sizeof(nfa_node_t)), 0);
	nfa_node_t *final = place_nfa_node_t(malloc(sizeof(nfa_node_t)), 0);
	nfa_node_t *node = literal_to_nfa_helper(initial, final, literal);

	add_nfa_initial(&nfa, initial);
	add_nfa_final(&nfa, final);

	vector_type(pnfa_node_t) nodes = setup_nodes_from_nfa(&nfa);
	destroy_vector(pnfa_node_t, &nodes); 
	return nfa;
}

nfa_t *make_monoextremal_nfa (nfa_t *nfa) {
	nfa_node_t *initial = place_nfa_node_t(malloc(sizeof(nfa_node_t)), 0);
	for ( size_t i=nfa->initial.begin; i<nfa->initial.end; ++i) {
		add_nfa_edge(initial, nfa->initial.data[i], 0); 
	}
	clear_nfa_initial(nfa);
	add_nfa_initial(nfa, initial);
	
	nfa_node_t *final = place_nfa_node_t(malloc(sizeof(nfa_node_t)), 0);
	for( size_t i=nfa->final.begin; i < nfa->final.end; ++i) {
		add_nfa_edge(nfa->final.data[i], final, 0);
	}
	clear_nfa_final(nfa);
	add_nfa_final(nfa, final);

	vector_type(pnfa_node_t) nodes = setup_nodes_from_nfa(nfa);
	destroy_vector(pnfa_node_t, &nodes); 

	return nfa;
}

nfa_t *make_avoidable_nfa (nfa_t *nfa) {
	make_monoextremal_nfa(nfa);
	add_nfa_edge(nfa->initial.data[0], nfa->final.data[0], 0);

	vector_type(pnfa_node_t) nodes = setup_nodes_from_nfa(nfa);
	destroy_vector(pnfa_node_t, &nodes); 
	return nfa;
}

nfa_t *make_repeatable_nfa (nfa_t *nfa) {
	make_monoextremal_nfa(nfa);
	add_nfa_edge(nfa->final.data[0], nfa->initial.data[0], 0);

	vector_type(pnfa_node_t) nodes = setup_nodes_from_nfa(nfa);
	destroy_vector(pnfa_node_t, &nodes); 
	return nfa;
}

nfa_t concatenation_nfa (const nfa_t *nfa_old1, const nfa_t *nfa_old2) {
	nfa_t nfa1, nfa2;
	place_nfa_t(&nfa1);
	place_nfa_t(&nfa2);

	copy_nfa_t(nfa_old1, &nfa1);
	copy_nfa_t(nfa_old2, &nfa2);

	make_monoextremal_nfa(&nfa1);
	make_monoextremal_nfa(&nfa2);
	
	add_nfa_edge(nfa1.final.data[0], nfa2.initial.data[0], 0);
	//nfa1.final.data[0]->flags &= ~flag_nfa_node_final;
	//nfa2.initial.data[0]->flags &= ~flag_nfa_node_initial;

	copy_vector(pnfa_node_t, &nfa2.final, &nfa1.final);
	destroy_nfa_t(&nfa2);

	vector_type(pnfa_node_t) nodes = setup_nodes_from_nfa(&nfa1);
	destroy_vector(pnfa_node_t, &nodes); 
	return nfa1;
}

nfa_t union_nfa (const nfa_t *nfa1, const nfa_t *nfa2) {
	nfa_t nfa;
	place_nfa_t(&nfa);
	copy_nfa_t(nfa1, &nfa);

	concatenate_vector(pnfa_node_t, &nfa.initial, &nfa2->initial);
	concatenate_vector(pnfa_node_t, &nfa.final, &nfa2->final);

	vector_type(pnfa_node_t) nodes = setup_nodes_from_nfa(&nfa);
	destroy_vector(pnfa_node_t, &nodes); 

	return nfa;
}

vector_pnfa_node_t_t dfs_with_action_nfa (const nfa_t *nfa, void (*action) (nfa_node_t *)) {
	uint64_t id=0;
	vector_pnfa_node_t_t visited = vector(pnfa_node_t, 0);
	vector_pnfa_node_t_t stack = vector(pnfa_node_t, 0);
	nfa_node_t *node = NULL;
	nfa_node_t *dest = NULL;
	vector_pnfa_node_t_t *out = NULL;

	for( size_t i = nfa->initial.begin; i < nfa->initial.end; ++i ) {
		node = nfa->initial.data[i];
		push_back_vector(pnfa_node_t, &stack, node);
		node->flags |= flag_nfa_node_visited;
		node->id = id++;
		push_back_vector(pnfa_node_t, &visited, node);
	}

	while (!is_empty_vector_pnfa_node_t_t(&stack)) {
		node = pop_back_vector(pnfa_node_t, &stack);

		for( size_t i = 0; i<256; ++i ) {
			out = node->out+i;
			for ( size_t j = out->begin; j < out->end; ++j ) {
				dest = out->data[j];
				if (dest->flags & flag_nfa_node_visited) {
					continue;
				}
				push_back_vector(pnfa_node_t, &stack, dest);
				dest->flags |= flag_nfa_node_visited;
				dest->id = id++;

				push_back_vector(pnfa_node_t, &visited, dest);
			}
		}

		action(node);
	}

	destroy_vector(pnfa_node_t, &stack);

	return visited;
}

void print_nfa_node (nfa_node_t *node) {
	vector_pnfa_node_t_t *out = NULL;

	printf("NODE %ld %s%s\n\n", 
		node->id, 
		node->flags&flag_nfa_node_initial ? "INITIAL " : "", 
		node->flags&flag_nfa_node_final ? "FINAL " : "");

	for( size_t i = 0; i<256; ++i ) {
		out = node->out+i;
		for ( size_t j = out->begin; j < out->end; ++j ) {
			printf("\t-- %ld = %c%s --> %ld\n", i, i ? (uint8_t)i : 'e', i ? "" : "mpty", out->data[j]->id);
		}
	}

	printf("\n");
}

void print_as_mermaid_body_nfa_node (nfa_node_t *node) {
	vector_pnfa_node_t_t *out = NULL;

	switch (node->flags & (flag_nfa_node_initial|flag_nfa_node_final)) {
	case 0:
		printf(" %ld(%ld)\n", node->id, node->id);
		break;
	case flag_nfa_node_initial:
		printf(" %ld(%ld)\n style %ld stroke:#a22,stroke-width:3px,stroke-dasharray: 5, 5\n", 
			node->id, 
			node->id, 
			node->id);
		break;
	case flag_nfa_node_final:
		printf(" %ld{%ld}\n style %ld fill:#f33,stroke-width:3px\n", 
			node->id, 
			node->id, 
			node->id);
		break;
	default:
		printf(" %ld{%ld}\n style %ld fill:#f33,stroke:#a22,stroke-width:3px,stroke-dasharray: 5, 5\n", 
			node->id, 
			node->id, 
			node->id);
		break;
	}

	for( size_t i = 0; i<256; ++i ) {
		out = node->out+i;
		for ( size_t j = out->begin; j < out->end; ++j ) {
			printf(" %ld --\" %ld = %c%s \"--> %ld\n", 
				node->id, 
				i, 
				i 
				? ((uint8_t)i != 34 ? (uint8_t)i : ' ')
				: 'e', 
				i ? "" : "mpty", 
				out->data[j]->id);
		}
	}
}

void dummy_print_nfa_node (nfa_node_t *node) {}

void clean_flags_of_nfa_node (nfa_node_t *node) {
	node->flags &= flag_nfa_node_visited;
}

void print_nfa (const nfa_t *nfa, void (*printer) (nfa_node_t *)) {
	if (!nfa) {
		return;
	}

	if (printer == print_as_mermaid_body_nfa_node) {
		//printf("{{#mermaid:graph TD\n"); //for mediawiki
		printf("graph TD\n");
	}

	vector_pnfa_node_t_t visited = dfs_with_action_nfa(nfa, printer);

	if (printer == print_as_mermaid_body_nfa_node) {
		//printf("}}\n");	//for mediawiki
		//printf("}\n");
	}

	for( size_t i = visited.begin; i < visited.end; ++i ) {
		visited.data[i]->flags &= ~flag_nfa_node_visited;
	}
	destroy_vector(pnfa_node_t, &visited);
}

vector_type(pnfa_node_t) get_copy_of_nodes (vector_type(pnfa_node_t) *node_ptrs) {
	vector_type(pnfa_node_t) nodes = vector(pnfa_node_t, node_ptrs->end-node_ptrs->begin);
	forall (node_ptrs, i) {
		nodes.data[i] = place_nfa_node_t(malloc(sizeof(nfa_node_t)), 0);
		nodes.data[i]->id = i;
	}

	nodes.end = node_ptrs->end-node_ptrs->begin;

	return nodes;
}

vector_type(pnfa_node_t) get_copy_with_edges_of_nodes (vector_type(pnfa_node_t) *node_ptrs) {
	vector_type(pnfa_node_t) nodes = get_copy_of_nodes(node_ptrs);

	forall (node_ptrs, i) {
		uint8_t c = 0;
		do {
			vector_type(pnfa_node_t) *dest_ptrs = node_ptrs->data[i]->out+c;
			forall(dest_ptrs, j) {
				add_nfa_edge(nodes.data[node_ptrs->data[i]->id], nodes.data[dest_ptrs->data[j]->id], c);
			}
		} while(c++ != 255);
	}

	return nodes;
}

void print_nodes (vector_type(pnfa_node_t) *nodes) {
	forall(nodes, i) {
		printf("i:%ld -- id:%ld\n", i, nodes->data[i]->id);
		uint8_t c = 0;
		do {
			vector_type(pnfa_node_t) *sources = nodes->data[i]->out+c;
			forall(sources, j) {
				printf("\t\t==%c==> %ld\n", c, sources->data[j]->id);
			}
		} while(c++ != 255);
	}
}

vector_type(pnfa_node_t) setup_nodes_from_nfa (nfa_t *_nfa) {
	vector_type(pnfa_node_t) nodes = dfs_with_action_nfa(_nfa, clean_flags_of_nfa_node);
	set_extremality_flags(_nfa);	
	unset_flags(&nodes, ~flag_nfa_node_visited);

	return nodes;
}

vector_type(pnfa_node_t) *setup_nfa_from_nodes (nfa_t *nfa, vector_type(pnfa_node_t) *nodes) {
	clear_vector(pnfa_node_t, &nfa->initial);
	clear_vector(pnfa_node_t, &nfa->final);
	forall (nodes, i) {
		if (nodes->data[i]->flags & flag_nfa_node_initial) {
			push_back_vector(pnfa_node_t, &nfa->initial, nodes->data[i]);
		}
		if (nodes->data[i]->flags & flag_nfa_node_final) {
			push_back_vector(pnfa_node_t, &nfa->final, nodes->data[i]);
		}
	}

	return nodes;

}

nfa_t *set_extremality_flags (nfa_t *nfa) {
	forall(&nfa->initial, i) {
		nfa->initial.data[i]->flags |= flag_nfa_node_initial;
	}

	forall(&nfa->final, i) {
		nfa->final.data[i]->flags |= flag_nfa_node_final;
	}

	return nfa;
}

vector_type(pnfa_node_t) *unset_flags (vector_type(pnfa_node_t) *nodes, uint64_t flags) {
	forall( nodes, i ) {
		nodes->data[i]->flags &= flags;
	}

	return nodes;
}

vector_type(pnfa_node_t) *set_flags (vector_type(pnfa_node_t) *nodes, uint64_t flags) {
	forall( nodes, i ) {
		nodes->data[i]->flags |= flags;
	}

	return nodes;	
}

vector_def(nfa_t);
