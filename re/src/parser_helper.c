#include "parser_helper.h"

static uint64_t checked_options;
string_t filename;
static const uint64_t 
	mdfa_mermaid = 1, 
	mdfa_gotocode = 2, 
	mdfa_match = 4, 
	searchmdfa_mermaid = 8, 
	searchmdfa_gotocode = 16, 
	searchmdfa_match = 32;

// (memory unsafe)
void set_options_from_command_line(int argc, char **argv) {
	checked_options = 0;
	place_string_t(&filename);

	for (int i=1; i<argc; ++i) {
		switch (argv[i][0]) {
		case 'm':
			switch (argv[i][6]) {
			case 'o':
				checked_options |= mdfa_gotocode;
				break;
			case 'e':
				checked_options |= mdfa_mermaid;
				break;
			case 'a':
				checked_options |= mdfa_match;
				break;
			default:
				break;
			}
			break;
		case 's':
			switch (argv[i][12]) {
			case 'o':
				checked_options |= searchmdfa_gotocode;
				break;
			case 'e':
				checked_options |= searchmdfa_mermaid;
				break;
			case 'a':
				checked_options |= searchmdfa_match;
				break;
			default:
				break;
			}
			break;
		case '-':
			switch (argv[i][1]) {
				case 'f':
					cat_cstring_string_t(&filename, argv[i]+3);
					break;
				default:
					break;
			}
			break;
		default:
			break;
		}
	}
}

void print_response (nfa_t nfa[static 1]) {
	static const uint64_t mdfa_checked = mdfa_gotocode | mdfa_mermaid | mdfa_match;
	static const uint64_t searchmdfa_checked = searchmdfa_gotocode | searchmdfa_mermaid | searchmdfa_match;
	
	#define separator(nfa_name) printf("\n~~~~~~~~~~\n" #nfa_name "\n")

	nfa_t mdfa; 
	place_nfa_t(&mdfa);
	vector_type(pnfa_node_t) mdfa_nodes = vector(pnfa_node_t, 0);

	nfa_t searchmdfa;
	place_nfa_t(&searchmdfa);
	vector_type(pnfa_node_t) searchmdfa_nodes = vector(pnfa_node_t, 0);

	nfa_t reversedfa;
	place_nfa_t(&reversedfa);
	vector_type(pnfa_node_t) reversedfa_nodes = vector(pnfa_node_t, 0);

	if (checked_options & mdfa_checked) {
		mdfa_nodes = brzozowski_minimization_of_nfa(nfa, &mdfa);
		if (checked_options & mdfa_gotocode) {
			separator(mdfa_gotocode);
			string_t code = dfa_goto_coder(&mdfa, "printer", "\tif ((c = getchar())==EOF) { return accepted_states; }\n", "");
			printf("%s\n", cstringify_string_t(&code));
			destroy_string_t(&code);
		}
		if (checked_options & mdfa_mermaid) {
			separator(mdfa_mermaid);
			print_nfa(&mdfa, print_as_mermaid_body_nfa_node);
		}
		if (checked_options & mdfa_match) {
			separator(mdfa_match);
		}
	}
	if (checked_options & searchmdfa_checked) {
		nfa_t *source;
		if (checked_options & mdfa_checked) {
			source = &mdfa;
		} else {
			source = nfa;
			// set id of nfa nodes
			vector_type(pnfa_node_t) nodes = dfs_with_action_nfa(nfa, dummy_print_nfa_node);
			unset_flags(&nodes, ~flag_nfa_node_visited);
			destroy_vector(pnfa_node_t, &nodes);
		}

		nfa_t searchnfa;
		place_nfa_t(&searchnfa);
		vector_type(pnfa_node_t) searchnfa_nodes = nfa_to_search_automata(source, &searchnfa);
		searchmdfa_nodes = brzozowski_minimization_of_nfa(&searchnfa, &searchmdfa);

		forall (&searchnfa_nodes, i) {
			destroy_nfa_node_t(searchnfa_nodes.data[i]);
			free(searchnfa_nodes.data[i]);
		}
		destroy_vector(pnfa_node_t, &searchnfa_nodes);
		destroy_nfa_t(&searchnfa);
		
		if (checked_options & searchmdfa_gotocode) {
			separator(searchmdfa_gotocode);
			string_t code = dfa_goto_coder(&searchmdfa, "printer", "\tif ((c = getchar())==EOF) { return accepted_states; }\n", "goto node0;");
			printf("%s\n", cstringify_string_t(&code));
			destroy_string_t(&code);
		}
		if (checked_options & searchmdfa_mermaid) {
			separator(searchmdfa_mermaid);
			print_nfa(&searchmdfa, print_as_mermaid_body_nfa_node);
		}
		if (checked_options & searchmdfa_match) {
			separator(searchmdfa_match);
			if (checked_options & mdfa_checked) {
				reversedfa_nodes = reverse_nfa(&mdfa, &reversedfa);
			} else {
				nfa_t dfa;
				vector_type(pnfa_node_t) dfa_nodes = nfa_to_dfa(nfa, &dfa);

				nfa_t rnfa;
				vector_type(pnfa_node_t) rnfa_nodes = reverse_nfa(&dfa, &rnfa);

				forall(&dfa_nodes, i) {
					destroy(nfa_node_t)(dfa_nodes.data[i]);
					free(dfa_nodes.data[i]);
				}
				destroy_vector(pnfa_node_t, &dfa_nodes);
				destroy(nfa_t)(&dfa);

				reversedfa_nodes = nfa_to_dfa(&rnfa, &reversedfa);

				forall(&rnfa_nodes, i) {
					destroy(nfa_node_t)(rnfa_nodes.data[i]);
					free(rnfa_nodes.data[i]);
				}
				destroy_vector(pnfa_node_t, &rnfa_nodes);
				destroy(nfa_t)(&rnfa);
			}

			string_t code = re_matcher_goto_coder(&searchmdfa_nodes, &reversedfa_nodes);
			printf("%s\n", cstringify_string_t(&code));
			destroy_string_t(&code);
		}
	}
 
	forall (&mdfa_nodes, i) {
		destroy_nfa_node_t(mdfa_nodes.data[i]);
		free(mdfa_nodes.data[i]);
	}
	destroy_vector(pnfa_node_t, &mdfa_nodes);
	destroy_nfa_t(&mdfa);

	forall (&searchmdfa_nodes, i) {
		destroy_nfa_node_t(searchmdfa_nodes.data[i]);
		free(searchmdfa_nodes.data[i]);
	}
	destroy_vector(pnfa_node_t, &searchmdfa_nodes);
	destroy_nfa_t(&searchmdfa);

	forall (&reversedfa_nodes, i) {
		destroy_nfa_node_t(reversedfa_nodes.data[i]);
		free(reversedfa_nodes.data[i]);
	}
	destroy_vector(pnfa_node_t, &reversedfa_nodes);
	destroy_nfa_t(&reversedfa);

	#undef separator
}