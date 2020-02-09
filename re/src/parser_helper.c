#include "parser_helper.h"

static uint64_t checked_options;
static const uint64_t 
	mdfa_mermaid = 1, 
	mdfa_gotocode = 2, 
	mdfa_search = 4, 
	suffixmdfa_mermaid = 8, 
	suffixmdfa_gotocode = 16, 
	suffixmdfa_search = 32;

// (memory unsafe)
void set_options_from_command_line(int argc, char **argv) {
	checked_options = 0;

	for (int i=1; i<argc; ++i) {
		switch (argv[i][0]) {
		case 'm':
			switch (argv[i][5]) {
			case 'g':
				checked_options |= mdfa_gotocode;
				break;
			case 'm':
				checked_options |= mdfa_mermaid;
				break;
			case 's':
				checked_options |= mdfa_search;
				break;
			default:
				break;
			}
			break;
		case 's':
			switch (argv[i][11]) {
			case 'g':
				checked_options |= suffixmdfa_gotocode;
				break;
			case 'm':
				checked_options |= suffixmdfa_mermaid;
				break;
			case 's':
				checked_options |= suffixmdfa_search;
				break;
			default:
				break;
			}
		default:
			break;
		}
	}
}

//dummy_print_nfa_node; //print_as_mermaid_body_nfa_node; //print_nfa_node;
void print_response (nfa_t *nfa) {
	static const uint64_t mdfa_checked = mdfa_gotocode | mdfa_mermaid | mdfa_search;
	static const uint64_t suffixmdfa_checked = suffixmdfa_gotocode | suffixmdfa_mermaid | suffixmdfa_search;
	
	#define separator(nfa_name) printf("\n~~~~~~~~~~\n" #nfa_name "\n")

	nfa_t mdfa; 
	place_nfa_t(&mdfa);
	vector_type(pnfa_node_t) mdfa_nodes = vector(pnfa_node_t, 0);

	nfa_t suffixmdfa;
	place_nfa_t(&suffixmdfa);
	vector_type(pnfa_node_t) suffixmdfa_nodes = vector(pnfa_node_t, 0);


	if (checked_options & mdfa_checked) {
		mdfa_nodes = brzozowski_minimization_of_nfa(nfa, &mdfa);
		if (checked_options & mdfa_gotocode) {
			separator(mdfa_gotocode);
			printf("%s\n", dfa_goto_coder(&mdfa, "printer", "getchar"));
		}
		if (checked_options & mdfa_mermaid) {
			separator(mdfa_mermaid);
			print_nfa(&mdfa, print_as_mermaid_body_nfa_node);
		}
		if (checked_options & mdfa_search) {
			separator(mdfa_search);
		}
	}
	if (checked_options & suffixmdfa_checked) {
		nfa_t suffixnfa;
		place_nfa_t(&suffixnfa);
		vector_type(pnfa_node_t) nodes = vector(pnfa_node_t, 0);

		if (checked_options & mdfa_checked) {
			copy_nfa_t(&mdfa, &suffixnfa);
			destroy_vector(pnfa_node_t, &suffixnfa.initial);
			copy_vector(pnfa_node_t, &mdfa_nodes, &suffixnfa.initial);
		} else {
			copy_nfa_t(nfa, &suffixnfa);
			destroy_vector(pnfa_node_t, &suffixnfa.initial);
			suffixnfa.initial = dfs_with_action_nfa(nfa, dummy_print_nfa_node);
		}
		set_flags(&suffixnfa.initial, flag_nfa_node_initial);
		unset_flags(&suffixnfa.initial, ~flag_nfa_node_visited);

		suffixmdfa_nodes = brzozowski_minimization_of_nfa(&suffixnfa, &suffixmdfa);
		destroy_nfa_t(&suffixnfa);

		if (checked_options & suffixmdfa_gotocode) {
			separator(suffixmdfa_gotocode);
			printf("%s\n", dfa_goto_coder(&suffixmdfa, "printer", "getchar"));
		}
		if (checked_options & suffixmdfa_mermaid) {
			separator(suffixmdfa_mermaid);
			print_nfa(&suffixmdfa, print_as_mermaid_body_nfa_node);
		}
		if (checked_options & suffixmdfa_search) {
			separator(suffixmdfa_search);
		}
	}
 
	forall (&mdfa_nodes, i) {
		destroy_nfa_node_t(mdfa_nodes.data[i]);
	}
	destroy_vector(pnfa_node_t, &mdfa_nodes);
	destroy_nfa_t(&mdfa);

	forall (&suffixmdfa_nodes, i) {
		destroy_nfa_node_t(suffixmdfa_nodes.data[i]);
	}
	destroy_vector(pnfa_node_t, &suffixmdfa_nodes);
	destroy_nfa_t(&suffixmdfa);
	
	#undef separator
}