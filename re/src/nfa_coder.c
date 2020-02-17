#include "nfa_coder.h"

string_t *dfa_node_goto_coder (
	string_t code[static 1], 
	nfa_node_t node[static 1], 
	const char *input, 
	const char *default_action) 
{
	char temp_buffer[50];
	cat_bytes_string_t(code, cum_size_literal("node"));

	sprintf(temp_buffer, "%lu", node->id);
	cat_cstring_string_t(code, temp_buffer);
	cat_bytes_string_t(code, cum_size_literal(":\n"));

	if (node->flags & flag_nfa_node_final) {
		cat_bytes_string_t(code, 
			cum_size_literal("\taccept(counter, "));
		cat_cstring_string_t(code, temp_buffer);
		cat_bytes_string_t(code, cum_size_literal(", &accepted_states);\n"));
	}

	cat_cstring_string_t(code, input);
	cat_bytes_string_t(code, cum_size_literal(
		"\t++counter;\n"
		"\tswitch(c) {\n"));

	uint8_t c = 0;
	while (c++ != 255) {
		if (is_empty_vector(pnfa_node_t, node->out+c)) {
			continue;
		}
		cat_bytes_string_t(code, cum_size_literal("\tcase "));
		sprintf(temp_buffer, "%u", c);
		cat_cstring_string_t(code, temp_buffer);
		cat_bytes_string_t(code, cum_size_literal(":\n"
					"\t\tgoto node"));
		sprintf(temp_buffer, "%lu", node->out[c].data[0]->id);
		cat_cstring_string_t(code, temp_buffer);
		cat_bytes_string_t(code, cum_size_literal(";\n"));
	}
	cat_bytes_string_t(code, cum_size_literal("\tdefault:\n\t\t"));
	cat_cstring_string_t(code, default_action);
	cat_bytes_string_t(code, cum_size_literal("\n\t\treturn accepted_states;\n\t}\n"));

	return code;
}

string_t *dfa_goto_coder_preamble (string_t code[static 1]) {
	cat_bytes_string_t(code, 
		cum_size_literal(
		"/* Generated GOTO code for DFA */\n"
		"/* Michele Miccinesi 2020      */\n"
		"\n"
		"#include \"dfazer.h\"\n"
		"\n"
		"typedef struct accepting_state_t {\n"
		"\tuint64_t counter, node_id;\n"
		"} accepting_state_t;\n"
		"\n"
		"#define destroy_accepting_state_t(state) \n"
		"vector_def_header(accepting_state_t);\n"
		"vector_def(accepting_state_t);\n"
		"\n"
		"\n"
		"vector_type(uint64_t) *accept(uint64_t counter, uint64_t node_id, vector_type(accepting_state_t) *accepted_states) {\n"
		"\tprintf(\"count:%lu, state:%lu\", counter, node_id);\n"
		"\taccepting_state_t state = {.counter=counter, .node_id=node_id};\n"
		"\tpush_back_vector(accepting_state_t, accepted_states, state);\n"
		"\treturn accepted_states;\n"
		"}\n"
		"\n"));
	return code;
}

string_t *dfa_goto_coder_body (
	string_t code[static 1],
	vector_type(pnfa_node_t) nodes[static 1],
	const char *name,
	const char *input,
	const char *default_action) 
{
	cat_bytes_string_t(code, cum_size_literal("vector_type(accepting_state_t) "));
	cat_cstring_string_t(code, name);
	cat_bytes_string_t(code, 
		cum_size_literal(
		"() {\n"
		"\tchar c;\n"
		"\tuint64_t counter = 0;\n"
		"\tvector_type(accepting_state_t) accepted_states = "
			"vector(accepting_state_t, 0);\n"
		"\n"));

	forall (nodes, i) {
			dfa_node_goto_coder(code, nodes->data[i], input, default_action);
	}
	cat_bytes_string_t(code, cum_size_literal("}"));

	return code;
}

char *dfa_goto_coder (nfa_t dfa[static 1], const char *name, const char *input, const char *default_action) {
	vector_type(pnfa_node_t) nodes = dfs_with_action_nfa(dfa, dummy_print_nfa_node);
	unset_flags(&nodes, ~flag_nfa_node_visited);
	string_t code;
	place_string_t(&code);

	dfa_goto_coder_preamble(&code);
	dfa_goto_coder_body(&code, &nodes, name, input, default_action);

	destroy_vector(pnfa_node_t, &nodes);
	return cstringify_string_t(&code);
}

// Assuming a non-empty DFA
// will unroll only if the DFA is acyclic
#define flag_nfa_node_visiting 8
vector_type(pnfa_node_t) unroll_dfa (nfa_t dag[static 1], nfa_t tree[static 1]) {
	place_nfa_t(tree);

	vector_type(pnfa_node_t) nodes = vector(pnfa_node_t, 0);
	vector_type(pnfa_node_t) stack_dag = vector(pnfa_node_t, 0);
	vector_type(pnfa_node_t) stack_tree = vector(pnfa_node_t, 0);

	uint64_t id = 0;
	push_back_vector(pnfa_node_t, &nodes, place_nfa_node_t(malloc(sizeof(nfa_node_t)), dag->initial.data[0]->flags));
	push_back_vector(pnfa_node_t, &stack_dag, dag->initial.data[0]);
	push_back_vector(pnfa_node_t, &stack_tree, nodes.data[0]);

	nfa_node_t *dag_node;
	nfa_node_t *tree_node;
	nfa_node_t *new_node;
	while (!is_empty_vector(pnfa_node_t, &stack_dag)) {
		dag_node = *back_vector(pnfa_node_t, &stack_dag);
		tree_node = *back_vector(pnfa_node_t, &stack_tree);

		if (dag_node->flags & flag_nfa_node_visiting) {
			dag_node->flags &= ~flag_nfa_node_visiting;
			pop_back_vector(pnfa_node_t, &stack_dag);
			pop_back_vector(pnfa_node_t, &stack_tree);
		} else {
			dag_node->flags |= flag_nfa_node_visiting;
			tree_node->id = id++;
			uint8_t c = 0;
			while (c++ != 255) {
				forall (dag_node->out+c, i) {
					// NOT A DAG!
					if (dag_node->out[c].data[i]->flags & flag_nfa_node_visiting) {
						forall(&nodes, j) {
							destroy_nfa_node_t(nodes.data[j]);
						}
						destroy_vector(pnfa_node_t, &nodes);
						destroy_vector(pnfa_node_t, &stack_dag);
						destroy_vector(pnfa_node_t, &stack_tree);
						return nodes;
					}
					push_back_vector(pnfa_node_t, &stack_dag, dag_node->out[c].data[i]);
					new_node = place_nfa_node_t(
						malloc(sizeof(nfa_node_t)), 
						dag_node->out[c].data[i]->flags & flag_nfa_node_final);
					push_back_vector(pnfa_node_t, &nodes, new_node);
					push_back_vector(pnfa_node_t, &stack_tree, new_node);
					add_nfa_edge(tree_node, new_node, c);
				}
			}
		}
	}

	add_nfa_initial(place_nfa_t(tree), nodes.data[0]);
	forall (&nodes, i) {
		if (nodes.data[i]->flags & flag_nfa_node_final) {
			add_nfa_final(tree, nodes.data[i]);
		}
	}

	destroy_vector(pnfa_node_t, &stack_dag);
	destroy_vector(pnfa_node_t, &stack_tree);
	return nodes;
}
