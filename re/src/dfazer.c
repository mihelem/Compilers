/* Author: Michele Miccinesi 02022020 			*/
/* DFA-zer: from NFA to DFA and minimization 	*/

#include "dfazer.h"


vector_def(vector_type(pnfa_node_t));

static vector_type(pnfa_node_t) *make_unique (vector_type(pnfa_node_t) *nodes) {
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

// @return: ptr to vector with unique ptrs to nfa nodes of the empty transition closure
vector_type(pnfa_node_t) *empty_trans_closure (vector_type(pnfa_node_t) *nodes) {
	make_unique(nodes);
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
}

heap_def(uint64_t, <, less);

// @return: ptr to vector of sorted ids in the subset
vector_type(uint64_t) *pnfa_nodes_to_ids (vector_type(pnfa_node_t) *in, vector_type(uint64_t) *out) {
	size_t begin = out->end;
	forall(in, i) {
		push_back_vector(uint64_t, out, in->data[i]->id);
	}
	heapify(uint64_t, less, out->data+begin, out->end-begin);
	heap_sort(uint64_t, less, out->data+begin, out->end-begin);

	return out;
}

/*
	@return: vector of ptrs to the nodes of the new DFA
	_nfa : ptr to original NFA
	_dfa :  ptr to new DFA
	id : next fresh id
	ids : trie, each new ids is indexed by the encoded respective subset
	new_id : ptr to the uint64_t in the trie where you specify the 
			corresponding new id
	subset_ids : temp to store ids of each subset
	subsets : vector of vector of pnfa_node_t 
			(each element is a vector of ptrs to nodes of a subset)
	bytes : temp to store the encoded subset
	node_ptr : temp 
	node_ptrs : vect to store ptrs to new nodes (~node_ptrs[id])
*/
vector_type(pnfa_node_t) nfa_to_dfa(nfa_t *_nfa, nfa_t *_dfa) {
	// the new nfa - a dfa indeed
	place_nfa_t(_dfa);
	// vector of subsets
	subset_dfa_node_t subsets = vector(vector_type(pnfa_node_t), 0);

	// first subset is the set of nodes reachable from the initial nodes 
	// with empty transitions
	push_back_vector(
		vector_type(pnfa_node_t), 
		&subsets, 
		vector(pnfa_node_t, 0));
	empty_trans_closure(
		concatenate_vector(pnfa_node_t, subsets.data, &_nfa->initial));
	// a subset is initial if it contains an initial node
	//             final                     final
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


	add_nfa_initial(_dfa, node_ptr);
	if (flags & flag_nfa_node_final) {
		add_nfa_final(_dfa, node_ptr);
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
					add_nfa_final(_dfa, node_ptr);
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
	destroy_vector(pnfa_node_t, &new_subset);

	return node_ptrs;
}

vector_type(pnfa_node_t) reverse_nfa(nfa_t *_nfa, nfa_t *r_nfa) {
	place_nfa_t(r_nfa);
	
	vector_type(pnfa_node_t) _node_ptrs = setup_nodes_from_nfa(_nfa);
	vector_type(pnfa_node_t) r_node_ptrs = get_copy_of_nodes(&_node_ptrs);

	forall(&_node_ptrs, i) {
		uint8_t c = 0;
		nfa_node_t *source_ptr = _node_ptrs.data[i];
		do {
			vector_type(pnfa_node_t) *dest_ptrs = source_ptr->out+c;
			forall(dest_ptrs, j) {
				nfa_node_t *dest_ptr = dest_ptrs->data[j];
				add_nfa_edge(r_node_ptrs.data[dest_ptr->id], r_node_ptrs.data[source_ptr->id], c);
			}
		} while(c++ != 255);
	}

	forall(&_nfa->initial, i) {
		push_back_vector(pnfa_node_t, &r_nfa->final, r_node_ptrs.data[_nfa->initial.data[i]->id]);
	}
	forall(&_nfa->final, i) {
		push_back_vector(pnfa_node_t, &r_nfa->initial, r_node_ptrs.data[_nfa->final.data[i]->id]);
	}

	set_extremality_flags(r_nfa);
	return r_node_ptrs;
}

vector_type(pnfa_node_t) brzozowski_minimization_of_nfa(nfa_t *nfa, nfa_t *mdfa) {
	nfa_t rnfa;
	vector_type(pnfa_node_t) node_ptrs1 = reverse_nfa(nfa, &rnfa);

	nfa_t drnfa;
	vector_type(pnfa_node_t) node_ptrs2 = nfa_to_dfa(&rnfa, &drnfa);
	
	forall(&node_ptrs1, i) {
		destroy(nfa_node_t)(node_ptrs1.data[i]);
	}
	destroy_vector(pnfa_node_t, &node_ptrs1);
	destroy(nfa_t)(&rnfa);

	nfa_t rdrnfa;
	vector_type(pnfa_node_t) node_ptrs3 = reverse_nfa(&drnfa, &rdrnfa);

	forall(&node_ptrs2, i) {
		destroy(nfa_node_t)(node_ptrs2.data[i]);
	}
	destroy_vector(pnfa_node_t, &node_ptrs2);
	destroy(nfa_t)(&drnfa);

	place_nfa_t(mdfa);
	vector_type(pnfa_node_t) node_ptrs4 = nfa_to_dfa(&rdrnfa, mdfa);

	forall(&node_ptrs3, i) {
		destroy(nfa_node_t)(node_ptrs3.data[i]);
	}
	destroy_vector(pnfa_node_t, &node_ptrs3);
	destroy(nfa_t)(&rdrnfa);

	return node_ptrs4;
}