/* Author: Michele Miccinesi 02022020 			*/
/* DFA-zer: from NFA to DFA and minimization 	*/

#include "dfazer.h"

// @return: ptr to vector with unique ptrs to nfa nodes of the empty transition closure
vector_type(pnfa_node_t) *empty_trans_closure (vector_type(pnfa_node_t) nodes[static 1]) {
	make_unique_nfa_nodes(nodes);
	nfa_node_t *node;
	forall( nodes, i ) {
		node = nodes->data[i];
		forall( node->out, j ) {
			if (node->out->data[j]->flags & flag_nfa_node_visited) {
				continue;
			}
			node->out->data[j]->flags |= flag_nfa_node_visited;
			push_back_vector(pnfa_node_t, nodes, node->out->data[j]);
		}
	}

	return nodes;
}

heap_def(uint64_t, <, less);

// @return: ptr to vector of sorted ids in the subset
vector_type(uint64_t) *pnfa_nodes_to_ids (const vector_type(pnfa_node_t) in[static 1], vector_type(uint64_t) out[static 1]) {
	size_t begin = out->end;
	forall(in, i) {
		push_back_vector(uint64_t, out, in->data[i]->id);
	}
	heapify(uint64_t, less, out->data+begin, out->end-begin);
	heap_sort(uint64_t, less, out->data+begin, out->end-begin);

	return out;
}

vector_type(pnfa_node_t) *add_initial_transform_subset_nfa (
	nfa_t in[static 1], 
	nfa_t _out[static 1], 
	vector_type(pnfa_node_t) subset[static 1]) 
{
	// For a better representation: rather than specifying all "go back to node 0", 
	// specify then a default action (go to 0 instead of fail)
	if (is_empty_vector(pnfa_node_t, subset)) {
		return subset;
	}
	return concatenate_vector(pnfa_node_t, subset, &in->initial);
}

vector_type(pnfa_node_t) *empty_transform_subset_nfa (
	nfa_t in[static 1], 
	nfa_t out[static 1], 
	vector_type(pnfa_node_t) subset[static 1]) 
{
	return subset;
}

vector_type(pnfa_node_t) nfa_to_dfa (nfa_t nfa[static 1], nfa_t dfa[static 1]) {
	return transformed_subset_construction_nfa(nfa, dfa, empty_transform_subset_nfa);
}

vector_type(pnfa_node_t) nfa_to_search_automata (nfa_t in[static 1], nfa_t out[static 1]) {
	return transformed_subset_construction_nfa(in, out, add_initial_transform_subset_nfa);
}

vector_type(pnfa_node_t) transformed_subset_construction_nfa(
	nfa_t in[static 1] , 
	nfa_t out[static 1] ,
	vector_type(pnfa_node_t) *subset_transform (nfa_t [static 1], nfa_t [static 1], vector_type(pnfa_node_t) [static 1])) 
{
	// the new nfa - a dfa indeed
	place_nfa_t(out);
	// vector of subsets
	subset_dfa_nodes_t subsets = vector(vector_type(pnfa_node_t), 0);

	// first subset is the set of nodes reachable from the initial nodes 
	// with empty transitions
	push_back_vector(
		vector_type(pnfa_node_t), 
		&subsets, 
		vector(pnfa_node_t, 0));
	empty_trans_closure(
		concatenate_vector(pnfa_node_t, subsets.data, &in->initial));
	// a subset is final if it contains a final node
	uint64_t id = 0;
	uint64_t flags = 0;
	forall(subsets.data, i) {
		flags |= subsets.data[id].data[i]->flags;
	}
	flags &= ~flag_nfa_node_visited;
	unset_flags(subsets.data+id, ~flag_nfa_node_visited);
	
	// represent each subset with the set of the corresponding ids
	vector_type(uint64_t) subset_ids = 
		vector(uint64_t, subsets.data->end-subsets.data->begin);
	pnfa_nodes_to_ids(subsets.data, &subset_ids);
	// encode a set of ids to a byte string, unique for each subset
	bytes_t bytes = vector(uint8_t, 0);
	encode_subset_to_bytes(&subset_ids, &bytes);
	
	// initialize a trie, use it to recognize already existing subsets
	// each final transition (129) point to the id of the new subset
	// which is the corresponding index in the vector subsets
	trie_t ids = trie(1);
	add_empty_node_to_trie(&ids);

	uint64_t *new_id = add_subset_to_trie(&ids, &bytes);
	*new_id = id++;


	// (@return) pointers to nodes of the dfa
	vector_type(pnfa_node_t) node_ptrs = vector(pnfa_node_t, 0);

	// pointer to a new node of the dfa
	nfa_node_t *node_ptr = 
		place_nfa_node_t(malloc(sizeof(nfa_node_t)), flags);
	push_back_vector(pnfa_node_t, &node_ptrs, node_ptr);

	add_nfa_initial(out, node_ptr);
	if (flags & flag_nfa_node_final) {
		add_nfa_final(out, node_ptr);
	}

	uint8_t c;
	vector_type(pnfa_node_t) new_subset = vector(pnfa_node_t, 0);
	forall(&subsets, subset_id) {
		c = 0;
		while ( c++ != 255) {
			vector_type(pnfa_node_t) *psubset = subsets.data+subset_id;
	
			// add to new_subset all the destination node ptrs reach with
			// the character c from the given psubset
			clear_vector(pnfa_node_t, &new_subset);
			forall(psubset, source_id) {
				nfa_node_t *source = psubset->data[source_id];
				forall(source->out+c, dest_id) {
					nfa_node_t *dest = source->out[c].data[dest_id];
					if (dest->flags & flag_nfa_node_visited) {
						continue;
					}
					dest->flags |= flag_nfa_node_visited;
					push_back_vector(pnfa_node_t, &new_subset, dest);
				}
			}
			subset_transform(in, out, &new_subset);
			if (new_subset.end == new_subset.begin) {
				continue;
			}
			empty_trans_closure(&new_subset);

			clear_vector(uint64_t, &subset_ids);
			pnfa_nodes_to_ids(&new_subset, &subset_ids);

			clear_vector(uint8_t, &bytes);
			encode_subset_to_bytes(&subset_ids, &bytes);

			new_id = add_subset_to_trie(&ids, &bytes);

			if (*new_id == ~0) {
				flags = 0;
				forall(&new_subset, i) {
					flags |= new_subset.data[i]->flags;
				}
				flags &= ~(flag_nfa_node_visited|flag_nfa_node_initial);

				push_back_vector(vector_type(pnfa_node_t), &subsets, vector(pnfa_node_t, 0));
				concatenate_vector(pnfa_node_t, subsets.data+subsets.end-1, &new_subset);
				
				node_ptr = place_nfa_node_t(malloc(sizeof(nfa_node_t)), flags);
				node_ptr->id = id;
				*new_id = id++;
				push_back_vector(pnfa_node_t, &node_ptrs, node_ptr);
				
				if (flags & flag_nfa_node_final) {
					add_nfa_final(out, node_ptr);
				}
			} else {
				node_ptr = node_ptrs.data[*new_id];
			}
			unset_flags(&new_subset, ~flag_nfa_node_visited);
			add_nfa_edge (node_ptrs.data[subset_id], node_ptr, c);
			//push_back_vector(pnfa_node_t, node_ptrs.data[subset_id]->out+c, node_ptr);
		}
	}

	/*printf("\n---------------\n SUBSETS:\n");
	forall(&subsets, i) {
		printf("\n------------\n");
		forall(&subsets.data[i], j) {
			printf("%lu\t", subsets.data[i].data[j]->id);
		}
	}
	printf("\n---------------\n");*/


	destroy_vector(vector_type(pnfa_node_t), &subsets);
	destroy_vector(uint64_t, &subset_ids);
	destroy_vector(uint8_t, &bytes);
	destroy_trie(&ids);
	destroy_vector(pnfa_node_t, &new_subset);

	return node_ptrs;
}

vector_type(pnfa_node_t) reverse_nfa(nfa_t  nfa[static 1], nfa_t  r_nfa[static 1]) {
	place_nfa_t(r_nfa);
	
	vector_type(pnfa_node_t) nodes = setup_nodes_from_nfa(nfa);
	vector_type(pnfa_node_t) r_nodes = get_copy_of_nodes(&nodes);

	forall (&nodes, i) {
		uint8_t c = 0;
		nfa_node_t *source = nodes.data[i];
		do {
			vector_type(pnfa_node_t) *dests = source->out+c;
			forall (dests, j) {
				nfa_node_t *dest = dests->data[j];
				add_nfa_edge(r_nodes.data[dest->id], r_nodes.data[source->id], c);
			}
		} while(c++ != 255);
	}

	forall(&nfa->initial, i) {
		push_back_vector(pnfa_node_t, &r_nfa->final, r_nodes.data[nfa->initial.data[i]->id]);
	}
	forall(&nfa->final, i) {
		push_back_vector(pnfa_node_t, &r_nfa->initial, r_nodes.data[nfa->final.data[i]->id]);
	}

	set_extremality_flags(r_nfa);
	return r_nodes;
}

vector_type(pnfa_node_t) brzozowski_minimization_of_nfa(nfa_t nfa[static 1] , nfa_t mdfa[static 1] ) {
	nfa_t rnfa;
	vector_type(pnfa_node_t) nodes1 = reverse_nfa(nfa, &rnfa);

	nfa_t drnfa;
	vector_type(pnfa_node_t) nodes2 = nfa_to_dfa(&rnfa, &drnfa);
	
	forall(&nodes1, i) {
		destroy(nfa_node_t)(nodes1.data[i]);
	}
	destroy_vector(pnfa_node_t, &nodes1);
	destroy(nfa_t)(&rnfa);

	nfa_t rdrnfa;
	vector_type(pnfa_node_t) nodes3 = reverse_nfa(&drnfa, &rdrnfa);

	forall(&nodes2, i) {
		destroy(nfa_node_t)(nodes2.data[i]);
	}
	destroy_vector(pnfa_node_t, &nodes2);
	destroy(nfa_t)(&drnfa);

	place_nfa_t(mdfa);
	vector_type(pnfa_node_t) nodes4 = nfa_to_dfa(&rdrnfa, mdfa);

	forall(&nodes3, i) {
		destroy(nfa_node_t)(nodes3.data[i]);
	}
	destroy_vector(pnfa_node_t, &nodes3);
	destroy(nfa_t)(&rdrnfa);

	return nodes4;
}

nfa_t intersection_dfazing_nfa (nfa_t nfa1[static 1], nfa_t nfa2[static 1]) {
	nfa_t dfa1, dfa2;
	place_nfa_t(&dfa1);
	place_nfa_t(&dfa2);

	vector_type(pnfa_node_t) nodes1 = nfa_to_dfa(nfa1, &dfa1);
	vector_type(pnfa_node_t) nodes2 = nfa_to_dfa(nfa2, &dfa2);

	nfa_t nfa = intersection_nfa(&dfa1, &dfa2);
	forall (&nodes1, i) {
		destroy_nfa_node_t (nodes1.data[i]);
	}
	forall (&nodes2, i) {
		destroy_nfa_node_t (nodes2.data[i]);
	}

	destroy_vector(pnfa_node_t, &nodes1);
	destroy_vector(pnfa_node_t, &nodes2);

	return nfa;
}