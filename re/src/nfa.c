/* Author: Michele Miccinesi 02022020 	*/
#include "nfa.h"

vector_def(pnfa_node_t);

nfa_node_t *place_nfa_node_t(nfa_node_t *node, const uint64_t flags) {
	if (!node) {
		perror("place_nfa_node_t on NULL\n");
		exit(EXIT_FAILURE);
	}
	node->id = 0;
	node->flags = flags;
	node->out = calloc(257, sizeof(vector_pnfa_node_t_t));
	if (!node->out) {
		perror("MEMORY SHORTAGE\n");
		exit(EXIT_FAILURE);
	}

	return node;
}

nfa_node_t make_nfa_node_t(const uint64_t flags) {
	nfa_node_t node;
	place_nfa_node_t(&node, flags);

	return node;
}

void destroy_nfa_node_t (nfa_node_t node[static 1]) {
	for( int i=0; i<257; ++i ) {
		destroy_vector(pnfa_node_t, node->out+i);
	}
	free(node->out);
	node->out = NULL;
}

nfa_t *place_nfa_t (nfa_t *nfa) {
	if (!nfa) {
		perror("place_nfa_t on NULL\n");
		exit(EXIT_FAILURE);
	}
	nfa->initial = vector(pnfa_node_t, 0);
	nfa->final = vector(pnfa_node_t, 0);

	return nfa;
}

void destroy_nfa_t (nfa_t nfa[static 1]) {
	destroy_vector(pnfa_node_t, &nfa->initial);
	destroy_vector(pnfa_node_t, &nfa->final);
}

nfa_t *destroy_nodes_of_nfa (nfa_t nfa[static 1]) {
	vector_type(pnfa_node_t) nodes = dfs_with_action_nfa(nfa, dummy_print_nfa_node);
	forall (&nodes, i) {
		destroy_nfa_node_t(nodes.data[i]);
		free(nodes.data[i]);
	}
	destroy_vector(pnfa_node_t, &nodes);

	clear_vector(pnfa_node_t, &nfa->initial);
	clear_vector(pnfa_node_t, &nfa->final);

	return nfa;
}

void copy_nfa_t (const nfa_t source[static 1], nfa_t dest[static 1]) {
	copy_vector(pnfa_node_t, &source->initial, &dest->initial);
	copy_vector(pnfa_node_t, &source->final, &dest->final);
}

void add_nfa_empty_transition (nfa_node_t source[static 1], nfa_node_t dest[static 1]) {
	push_back_vector(pnfa_node_t, source->out+256, dest);
}

void add_nfa_edge (nfa_node_t source[static 1], nfa_node_t dest[static 1], const size_t label) {
	push_back_vector(pnfa_node_t, source->out+label, dest);
}

void add_nfa_initial (nfa_t nfa[static 1], nfa_node_t initial[static 1]) {
	push_back_vector(pnfa_node_t, &nfa->initial, initial);
}

void add_nfa_final (nfa_t nfa[static 1], nfa_node_t final[static 1]) {
	push_back_vector(pnfa_node_t, &nfa->final, final);
}

void clear_nfa_initial (nfa_t nfa[static 1]) {
	clear_vector(pnfa_node_t, &nfa->initial);
}

void clear_nfa_final (nfa_t nfa[static 1]) {
	clear_vector(pnfa_node_t, &nfa->final);
}

nfa_t range_to_nfa (range_t range[static 1]) {
	nfa_t nfa;
	place_nfa_t(&nfa);
	
	nfa_node_t *initial = place_nfa_node_t(malloc(sizeof(nfa_node_t)), 0);
	nfa_node_t *final = place_nfa_node_t(malloc(sizeof(nfa_node_t)), 0);

	uint8_t c = 0;
	do {
		if (is_in_range(range, c)) {
			add_nfa_edge(initial, final, c);
		}
	} while (c++ < 255);

	add_nfa_initial(&nfa, initial);
	add_nfa_final(&nfa, final);

	set_flags(&nfa.initial, flag_nfa_node_initial);
	set_flags(&nfa.final, flag_nfa_node_final);

	return nfa;
}

nfa_t literal_to_nfa (const uint8_t literal) {
	nfa_t nfa;
	place_nfa_t(&nfa);

	nfa_node_t *initial = place_nfa_node_t(malloc(sizeof(nfa_node_t)), flag_nfa_node_initial);
	nfa_node_t *final = place_nfa_node_t(malloc(sizeof(nfa_node_t)), flag_nfa_node_final);
	add_nfa_edge(initial, final, literal);

	add_nfa_initial(&nfa, initial);
	add_nfa_final(&nfa, final);

	return nfa;
}

nfa_t *make_monoextremal_nfa (nfa_t nfa[static 1]) {
	nfa_node_t *initial = place_nfa_node_t(malloc(sizeof(nfa_node_t)), flag_nfa_node_initial);
	forall (&nfa->initial, i) {
		add_nfa_empty_transition(initial, nfa->initial.data[i]); 
	}
	unset_flags(&nfa->initial, ~flag_nfa_node_initial);
	clear_nfa_initial(nfa);
	add_nfa_initial(nfa, initial);
	
	nfa_node_t *final = place_nfa_node_t(malloc(sizeof(nfa_node_t)), flag_nfa_node_final);
	forall (&nfa->final, i) {
		add_nfa_empty_transition(nfa->final.data[i], final);
	}
	unset_flags(&nfa->final, ~flag_nfa_node_final);
	clear_nfa_final(nfa);
	add_nfa_final(nfa, final);

	return nfa;
}

nfa_t *make_avoidable_nfa (nfa_t nfa[static 1]) {
	make_monoextremal_nfa(nfa);
	add_nfa_empty_transition(nfa->initial.data[0], nfa->final.data[0]);

	return nfa;
}

nfa_t *make_repeatable_nfa (nfa_t nfa[static 1]) {
	make_monoextremal_nfa(nfa);
	add_nfa_empty_transition(nfa->final.data[0], nfa->initial.data[0]);

	return nfa;
}

nfa_t concatenation_nfa (const nfa_t nfa_old1[static 1], const nfa_t nfa_old2[static 1]) {
	nfa_t nfa1, nfa2;
	place_nfa_t(&nfa1);
	place_nfa_t(&nfa2);

	copy_nfa_t(nfa_old1, &nfa1);
	copy_nfa_t(nfa_old2, &nfa2);

	make_monoextremal_nfa(&nfa1);
	make_monoextremal_nfa(&nfa2);
	
	add_nfa_empty_transition(nfa1.final.data[0], nfa2.initial.data[0]);
	//nfa1.final.data[0]->flags &= ~flag_nfa_node_final;
	//nfa2.initial.data[0]->flags &= ~flag_nfa_node_initial;

	copy_vector(pnfa_node_t, &nfa2.final, &nfa1.final);
	destroy_nfa_t(&nfa2);

	vector_type(pnfa_node_t) nodes = setup_nodes_from_nfa(&nfa1);
	destroy_vector(pnfa_node_t, &nodes); 
	return nfa1;
}

nfa_t *concatenate_nfa (nfa_t nfa[static 1], nfa_t nfa1[static 1], nfa_t nfa2[static 1]) {
	nfa->initial = nfa1->initial;
	nfa->final = nfa2->final;
	make_monoextremal_nfa(nfa1);
	forall( &nfa2->initial, i ) {
		add_nfa_empty_transition(nfa1->final.data[0], nfa2->initial.data[i]);
	}
	unset_flags(&nfa1->final, ~flag_nfa_node_final);
	destroy_vector(pnfa_node_t, &nfa1->final);

	unset_flags(&nfa2->initial, ~flag_nfa_node_initial);
	destroy_vector(pnfa_node_t, &nfa2->initial);

	nfa->initial = nfa1->initial;
	nfa1->initial = vector(pnfa_node_t, 0);
	nfa->final = nfa2->final;
	nfa2->final = vector(pnfa_node_t, 0);

	set_flags(&nfa->initial, flag_nfa_node_initial);
	set_flags(&nfa->final, flag_nfa_node_final);

	return nfa;
}

nfa_t union_nfa (const nfa_t nfa1[static 1], const nfa_t nfa2[static 1]) {
	nfa_t nfa;
	place_nfa_t(&nfa);
	copy_nfa_t(nfa1, &nfa);

	concatenate_vector(pnfa_node_t, &nfa.initial, &nfa2->initial);
	concatenate_vector(pnfa_node_t, &nfa.final, &nfa2->final);

	vector_type(pnfa_node_t) nodes = setup_nodes_from_nfa(&nfa);
	destroy_vector(pnfa_node_t, &nodes); 

	return nfa;
}

nfa_t *unite_nfa (nfa_t nfa[static 1], nfa_t nfa1[static 1], nfa_t nfa2[static 1]) {
	concatenate_vector(pnfa_node_t, &nfa1->initial, &nfa2->initial);
	concatenate_vector(pnfa_node_t, &nfa1->final, &nfa2->final);
	nfa->initial = nfa1->initial;
	nfa->final = nfa1->final;

	nfa1->initial = vector(pnfa_node_t, 0);
	nfa1->final = vector(pnfa_node_t, 0);
	destroy_vector(pnfa_node_t, &nfa2->initial);
	destroy_vector(pnfa_node_t, &nfa2->final);

	return nfa;
}

vector_pnfa_node_t_t dfs_with_action_nfa (nfa_t nfa[static 1], void action (nfa_node_t *)) {
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

		for( size_t i = 0; i<257; ++i ) {
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

void print_nfa_node (nfa_node_t node[static 1]) {
	vector_pnfa_node_t_t *out = NULL;

	printf("NODE %ld %s%s\n\n", 
		node->id, 
		node->flags&flag_nfa_node_initial ? "INITIAL " : "", 
		node->flags&flag_nfa_node_final ? "FINAL " : "");

	for( size_t i = 0; i<257; ++i ) {
		out = node->out+i;
		forall (out, j) {
			printf("\t-- %ld = %s --> %ld\n", i, symbol_repr[i], out->data[j]->id);
		}
	}

	printf("\n");
}

void print_as_mermaid_body_nfa_node (nfa_node_t node[static 1]) {
	vector_type(pnfa_node_t) *out = NULL;

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

	for( size_t i = 0; i<257; ++i ) {
		out = node->out+i;
		forall (out, j) {
			printf(" %ld --\" %ld = %s \"--> %ld\n", node->id, i, symbol_repr[i], out->data[j]->id);
		}
	}
}

void dummy_print_nfa_node (nfa_node_t *_node) {}

void clean_flags_of_nfa_node (nfa_node_t node[static 1]) {
	node->flags &= flag_nfa_node_visited;
}

void print_nfa (nfa_t nfa[static 1], void printer (nfa_node_t *)) {
	if (printer == print_as_mermaid_body_nfa_node) {
		//printf("{{#mermaid:graph TD\n"); //for mediawiki
		printf("graph TD\n");
	}

	vector_pnfa_node_t_t visited = dfs_with_action_nfa(nfa, printer);

	if (printer == print_as_mermaid_body_nfa_node) {
		//printf("}}\n");	//for mediawiki
		//printf("}\n");
	}

	unset_flags(&visited, ~flag_nfa_node_visited);

	destroy_vector(pnfa_node_t, &visited);
}

vector_type(pnfa_node_t) get_copy_of_nodes (const vector_type(pnfa_node_t) node_ptrs[static 1]) {
	vector_type(pnfa_node_t) nodes = vector(pnfa_node_t, node_ptrs->end-node_ptrs->begin);
	forall (node_ptrs, i) {
		nodes.data[i] = place_nfa_node_t(malloc(sizeof(nfa_node_t)), 0);
		nodes.data[i]->id = i;
	}

	nodes.end = node_ptrs->end-node_ptrs->begin;

	return nodes;
}

vector_type(pnfa_node_t) get_copy_with_edges_of_nodes (const vector_type(pnfa_node_t) node_ptrs[static 1]) {
	vector_type(pnfa_node_t) nodes = get_copy_of_nodes(node_ptrs);

	forall (node_ptrs, i) {
		for (size_t l = 0; l<257; ++l) {
			vector_type(pnfa_node_t) *dest_ptrs = node_ptrs->data[i]->out+l;
			forall(dest_ptrs, j) {
				add_nfa_edge(nodes.data[node_ptrs->data[i]->id], nodes.data[dest_ptrs->data[j]->id], l);
			}
		}
	}

	return nodes;
}

void print_nodes (vector_type(pnfa_node_t) nodes[static 1]) {
	forall(nodes, i) {
		printf("i:%ld -- id:%ld\n", i, nodes->data[i]->id);
		for (size_t l = 0; l<257; ++l) {
			vector_type(pnfa_node_t) *sources = nodes->data[i]->out+l;
			forall(sources, j) {
				printf("\t\t==%s==> %ld\n", symbol_repr[l], sources->data[j]->id);
			}
		}
	}
}

vector_type(pnfa_node_t) setup_nodes_from_nfa (nfa_t nfa[static 1]) {
	vector_type(pnfa_node_t) nodes = dfs_with_action_nfa(nfa, clean_flags_of_nfa_node);
	set_extremality_flags(nfa);	
	unset_flags(&nodes, ~flag_nfa_node_visited);

	return nodes;
}

vector_type(pnfa_node_t) *setup_nfa_from_nodes (nfa_t nfa[static 1], vector_type(pnfa_node_t) nodes[static 1]) {
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

nfa_t *set_extremality_flags (nfa_t nfa[static 1]) {
	forall(&nfa->initial, i) {
		nfa->initial.data[i]->flags |= flag_nfa_node_initial;
	}

	forall(&nfa->final, i) {
		nfa->final.data[i]->flags |= flag_nfa_node_final;
	}

	return nfa;
}

vector_type(pnfa_node_t) *unset_flags (vector_type(pnfa_node_t) nodes[static 1], const uint64_t flags) {
	forall( nodes, i ) {
		nodes->data[i]->flags &= flags;
	}

	return nodes;
}

vector_type(pnfa_node_t) *set_flags (vector_type(pnfa_node_t) nodes[static 1], const uint64_t flags) {
	forall( nodes, i ) {
		nodes->data[i]->flags |= flags;
	}

	return nodes;	
}

nfa_t *apply_flags_to_nfa (nfa_t nfa[static 1], const uint64_t flags, const uint64_t unflags) {
	vector_type(pnfa_node_t) nodes =
		dfs_with_action_nfa(nfa, dummy_print_nfa_node);

	clear_vector(pnfa_node_t, &nfa->initial);
	clear_vector(pnfa_node_t, &nfa->final);
	forall (&nodes, i) {
		nodes.data[i]->flags |= flags;
		nodes.data[i]->flags &= unflags;
		if (nodes.data[i]->flags & flag_nfa_node_final) {
			push_back_vector(pnfa_node_t, &nfa->final, nodes.data[i]);
		}
		if (nodes.data[i]->flags & flag_nfa_node_initial) {
			push_back_vector(pnfa_node_t, &nfa->initial, nodes.data[i]);
		}
	}
	destroy_vector(pnfa_node_t, &nodes);

	return nfa;
}

vector_type(pnfa_node_t) *make_unique_nfa_nodes (vector_type(pnfa_node_t) nodes[static 1]) {
	unset_flags(nodes, ~flag_nfa_node_visited);
	size_t j = nodes->begin;
	forall(nodes, i) {
		if (nodes->data[i]->flags & flag_nfa_node_visited) {
			continue;
		}
		nodes->data[i]->flags |= flag_nfa_node_visited;
		nodes->data[j++] = nodes->data[i];
	}
	nodes->end = j;

	return nodes;
}

vector_def(nfa_t);

vector_def(vector_type(pnfa_node_t));

nfa_t intersection_nfa (const nfa_t nfa1[static 1], const nfa_t nfa2[static 1]) {
	vector_type(pnfa_node_t) couples = vector(pnfa_node_t, 0);
	vector_type(pnfa_node_t) nodes = vector(pnfa_node_t, 0);

	// couple: container for an id couple
	vector_type(uint64_t) couple = vector(uint64_t, 0);
	couple.end = 2;
	uint64_t couple_data[2];
	couple.data = couple_data;

	nfa_t nfa;
	place_nfa_t(&nfa);

	uint64_t flags = 0;
	uint64_t id = 0;

	bytes_t bytes = vector(uint8_t, 0);
	trie_t ids = trie(1);
	add_empty_node_to_trie(&ids);
	uint64_t *new_id;

	pnfa_node_t 
		node = NULL,
		node1 = NULL,
		node2 = NULL,
		new_node1 = NULL,
		new_node2 = NULL,
		new_node = NULL;

	forall (&nfa1->initial, i) {
		forall (&nfa2->initial, j) {
			new_node1 = nfa1->initial.data[i];
			new_node2 = nfa2->initial.data[j];

			push_back_vector(pnfa_node_t, &couples, new_node1);
			push_back_vector(pnfa_node_t, &couples, new_node2);

			couple_data[0] = new_node1->id;
			couple_data[1] = new_node2->id;
			encode_subset_to_bytes(&couple, &bytes);
			

			new_id = add_subset_to_trie(&ids, &bytes);
			*new_id = id++;

			flags = new_node1->flags & new_node2->flags;
			new_node = place_nfa_node_t(malloc(sizeof(nfa_node_t)), flags);
			new_node->id = *new_id;
			push_back_vector(pnfa_node_t, &nodes, new_node);

			add_nfa_initial(&nfa, new_node);
			if (flags & flag_nfa_node_final) {
				add_nfa_final(&nfa, new_node);
			}
		}
	}

	forall (&nodes, i) {
		node1 = couples.data[2*i];
		node2 = couples.data[2*i+1];
		node = nodes.data[i];
		
		for (size_t l=0; l<257; ++l) {
			forall (node1->out+l, j) {
				new_node1 = node1->out[l].data[j];
				forall (node2->out+l, k) {
					new_node2 = node2->out[l].data[k];

					couple_data[0] = new_node1->id;
					couple_data[1] = new_node2->id;
					
					clear_vector(uint8_t, &bytes);
					encode_subset_to_bytes(&couple, &bytes);

					new_id = add_subset_to_trie(&ids, &bytes);

					if (*new_id == ~0) {
						*new_id = id++;

						flags = new_node1->flags & new_node2->flags;

						new_node = place_nfa_node_t(malloc(sizeof(nfa_node_t)), flags);
						new_node->id = *new_id;
						if (flags & flag_nfa_node_final) {
							add_nfa_final(&nfa, new_node);
						}
						push_back_vector(pnfa_node_t, &nodes, new_node);

						push_back_vector(pnfa_node_t, &couples, new_node1);
						push_back_vector(pnfa_node_t, &couples, new_node2);
					} else {
						new_node = nodes.data[*new_id];
					}
					add_nfa_edge(node, new_node, l);
				}
			}
		}
	}

	destroy_vector(pnfa_node_t, &couples);
	destroy_vector(pnfa_node_t, &nodes);
	destroy_vector(uint8_t, &bytes);
	destroy_trie(&ids);

	return nfa;
}