#include "parser_helper.h"

static uint64_t checked_options;
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
		default:
			break;
		}
	}
}

void print_response (nfa_t *nfa) {
	static const uint64_t mdfa_checked = mdfa_gotocode | mdfa_mermaid | mdfa_match;
	static const uint64_t searchmdfa_checked = searchmdfa_gotocode | searchmdfa_mermaid | searchmdfa_match;
	
	#define separator(nfa_name) printf("\n~~~~~~~~~~\n" #nfa_name "\n")

	nfa_t mdfa; 
	place_nfa_t(&mdfa);
	vector_type(pnfa_node_t) mdfa_nodes = vector(pnfa_node_t, 0);

	nfa_t searchmdfa;
	place_nfa_t(&searchmdfa);
	vector_type(pnfa_node_t) searchmdfa_nodes = vector(pnfa_node_t, 0);

	if (checked_options & mdfa_checked) {
		mdfa_nodes = brzozowski_minimization_of_nfa(nfa, &mdfa);
		if (checked_options & mdfa_gotocode) {
			separator(mdfa_gotocode);
			printf("%s\n", dfa_goto_coder(&mdfa, "printer", "getchar", ""));
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
		nfa_t *source = 
			checked_options & mdfa_checked
			? &mdfa
			: nfa;

		nfa_t searchnfa;
		place_nfa_t(&searchnfa);
		vector_type(pnfa_node_t) searchnfa_nodes = nfa_to_search_automata(source, &searchnfa);
		searchmdfa_nodes = brzozowski_minimization_of_nfa(&searchnfa, &searchmdfa);

		forall (&searchnfa_nodes, i) {
			destroy_nfa_node_t(searchnfa_nodes.data[i]);
		}
		destroy_vector(pnfa_node_t, &searchnfa_nodes);
		destroy_nfa_t(&searchnfa);
		
		if (checked_options & searchmdfa_gotocode) {
			separator(searchmdfa_gotocode);
			printf("%s\n", dfa_goto_coder(&searchmdfa, "printer", "getchar", "goto node0;"));
		}
		if (checked_options & searchmdfa_mermaid) {
			separator(searchmdfa_mermaid);
			print_nfa(&searchmdfa, print_as_mermaid_body_nfa_node);
		}
		if (checked_options & searchmdfa_match) {
			separator(searchmdfa_match);
		}
	}
 
	forall (&mdfa_nodes, i) {
		destroy_nfa_node_t(mdfa_nodes.data[i]);
	}
	destroy_vector(pnfa_node_t, &mdfa_nodes);
	destroy_nfa_t(&mdfa);

	forall (&searchmdfa_nodes, i) {
		destroy_nfa_node_t(searchmdfa_nodes.data[i]);
	}
	destroy_vector(pnfa_node_t, &searchmdfa_nodes);
	destroy_nfa_t(&searchmdfa);

	#undef separator
}