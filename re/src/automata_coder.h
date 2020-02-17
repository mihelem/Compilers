#include "nfa_coder.h"

typedef struct goto_coder_options_t {
	string_t preamble_code;
	string_t automata_def_code;
	string_t automata_preamble_code;
	string_t initial_node_action_code;
	string_t final_node_action_code;
	string_t node_action_code;
	string_t node_default_transition_code;
	string_t automata_closure_code;
} goto_coder_options_t;

goto_coder_options_t *place_goto_coder_options_t (goto_coder_options_t options[static 1]);
goto_coder_options_t *destroy_goto_coder_options_t (goto_coder_options_t options[static 1]);

string_t *automata_from_options_node_goto_coder (
	nfa_node_t node[static 1],
	string_t code[static 1],
	goto_coder_options_t const options[static 1]);

string_t *automata_from_options_goto_coder (
	vector_type(pnfa_node_t) nodes[static 1],
	string_t *node_coder(
		nfa_node_t node[static 1], 
		string_t code[static 1], 
		goto_coder_options_t const options[static 1]),
	string_t code[static 1],
	goto_coder_options_t const options[static 1]);

string_t re_matcher_goto_coder(
	vector_type(pnfa_node_t) searchmdfa_nodes[static 1],
	vector_type(pnfa_node_t) reversedfa_nodes[static 1]);
	/*
	nfa_t searchmdfa[static 1], 
	nfa_t reverse_dfa[static 1]);
	*/