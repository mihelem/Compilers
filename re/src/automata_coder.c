#include "automata_coder.h"

goto_coder_options_t *place_goto_coder_options_t (goto_coder_options_t options[static 1])
{
	place_string_t(&options->preamble_code);
	place_string_t(&options->automata_def_code);
	place_string_t(&options->automata_preamble_code);
	place_string_t(&options->initial_node_action_code);
	place_string_t(&options->final_node_action_code);
	place_string_t(&options->node_action_code);
	place_string_t(&options->node_default_transition_code);
	place_string_t(&options->automata_closure_code);

	return options;
}

goto_coder_options_t *destroy_goto_coder_options_t (goto_coder_options_t options[static 1])
{
	destroy_string_t(&options->preamble_code);
	destroy_string_t(&options->automata_def_code);
	destroy_string_t(&options->automata_preamble_code);
	destroy_string_t(&options->initial_node_action_code);
	destroy_string_t(&options->final_node_action_code);
	destroy_string_t(&options->node_action_code);
	destroy_string_t(&options->node_default_transition_code);
	destroy_string_t(&options->automata_closure_code);

	return options;
}

string_t *automata_from_options_node_goto_coder (
	nfa_node_t node[static 1],
	string_t code[static 1],
	goto_coder_options_t const options[static 1])
{
	char temp_buffer[50];
	cat_bytes_string_t(code, cum_size_literal("node"));

	sprintf(temp_buffer, "%lu", node->id);
	cat_cstring_string_t(code, temp_buffer);
	cat_bytes_string_t(code, cum_size_literal(":\n"));

	if (node->flags & flag_nfa_node_initial) {
		cat_string_t(code, &options->initial_node_action_code);
	}
	if (node->flags & flag_nfa_node_final) {
		cat_string_t(code, &options->final_node_action_code);
	}
	cat_string_t(code, &options->node_action_code);

	cat_bytes_string_t(code, 
		cum_size_literal("\tswitch(c) {\n"));

	uint8_t c = 0;
	while (c++ != 255) {
		if (is_empty_vector(pnfa_node_t, node->out+c)) {
			continue;
		}
		cat_bytes_string_t(code, cum_size_literal("\tcase "));
		sprintf(temp_buffer, "%u", c);
		cat_cstring_string_t(code, temp_buffer);
		cat_bytes_string_t(code, 
			cum_size_literal(
			":\n"
			"\t\tgoto node"));
		sprintf(temp_buffer, "%lu", node->out[c].data[0]->id);
		cat_cstring_string_t(code, temp_buffer);
		cat_bytes_string_t(code, cum_size_literal(";\n"));
	}
	cat_bytes_string_t(code, cum_size_literal("\tdefault:\n"));
	cat_string_t(code, &options->node_default_transition_code);
	cat_bytes_string_t(code, cum_size_literal("\t}\n"));

	return code;
}

string_t *automata_from_options_goto_coder (
	vector_type(pnfa_node_t) nodes[static 1],
	string_t *node_coder(
		nfa_node_t node[static 1], 
		string_t code[static 1], 
		goto_coder_options_t const options[static 1]),
	string_t code[static 1],
	goto_coder_options_t const options[static 1]) 
{
	cat_string_t(code, &options->preamble_code);
	cat_string_t(code, &options->automata_def_code);
	cat_bytes_string_t(code, cum_size_literal("{\n"));
	cat_string_t(code, &options->automata_preamble_code);

	forall (nodes, i) {
		node_coder(nodes->data[i], code, options);
	}
	cat_bytes_string_t(code, cum_size_literal("}\n"));

	return code;
}

string_t re_matcher_goto_coder(
	vector_type(pnfa_node_t) searchmdfa_nodes[static 1],
	vector_type(pnfa_node_t) reversedfa_nodes[static 1])
/*	nfa_t searchmdfa[static 1], 
	nfa_t reversedfa[static 1])
*/
{
	string_t code;
	place_string_t(&code);

	goto_coder_options_t searchdfa_options;
	place_goto_coder_options_t(&searchdfa_options);

	cat_bytes_string_t(&searchdfa_options.preamble_code, 
		cum_size_literal(
		"/* Generated GOTO code for DFA */\n"
		"/* Michele Miccinesi 2020      */\n"
		"\n"
		"#include \"string_t.h\"\n"
		"#include \"vector_macros.h\"\n"
		"\n"
		"vector_def_header(size_t);\n"
		"vector_def(size_t);\n"
		"\n"
		"typedef struct matches_t {\n"
		"	vector_type(size_t) begins,\n"
		"						ends;\n"
		"} matches_t;\n"
		"\n"
		"matches_t *destroy_matches_t (matches_t matches[static 1]) {\n"
		"	destroy_vector(size_t, &matches->begins);\n"
		"	destroy_vector(size_t, &matches->ends);\n"
		"\n"
		"	return matches;\n"
		"}\n"
		"\n"
		"matches_t *place_matches_t (matches_t matches[static 1]) {\n"
		"	matches->begins = vector(size_t, 0);\n"
		"	matches->ends = vector(size_t, 0);\n"
		"\n"
		"	return matches;\n"
		"}\n"
		"\n"
		"vector_def_header(matches_t);\n"
		"vector_def(matches_t);\n"));

	cat_bytes_string_t(&searchdfa_options.automata_def_code,
		cum_size_literal(
		"vector_type(size_t) search_automata (const uint8_t *buffer, size_t end)"));

	cat_bytes_string_t(&searchdfa_options.automata_preamble_code,
		cum_size_literal(
		"\tuint8_t c;\n"
		"\tsize_t counter = 0;\n"
		"\tvector_type(size_t) ends = vector(size_t, 0);\n"));

	cat_bytes_string_t(&searchdfa_options.final_node_action_code,
		cum_size_literal(
		"\tpush_back_vector(size_t, &ends, counter);\n"));
	
	cat_bytes_string_t(&searchdfa_options.node_action_code,
		cum_size_literal(
		"\tif (counter == end) { return ends; }\n"
		"\tc = buffer[counter];\n"
		"\t++counter;\n"));

	cat_bytes_string_t(&searchdfa_options.node_default_transition_code,
		cum_size_literal(
		"\t\tgoto node0;\n"));

	goto_coder_options_t reversedfa_options;
	place_goto_coder_options_t(&reversedfa_options);

	cat_bytes_string_t(&reversedfa_options.automata_def_code,
		cum_size_literal(
		"matches_t reverse_automata (\n"
		"	uint8_t const *buffer,\n"
		"	vector_type(size_t) ends[static 1],\n"
		"	size_t const end_i)"));

	cat_bytes_string_t(&reversedfa_options.automata_preamble_code,
		cum_size_literal(
		"\tuint8_t c;\n"
		"\tsize_t 	counter = 0;\n"
		"\tsize_t const end = ends->data[end_i];\n"
		"\tsize_t ends_counter = 0;\n"
		"\n"
		"\tmatches_t matches;\n"
		"\tplace_matches_t(&matches);\n"));

	cat_bytes_string_t(&reversedfa_options.initial_node_action_code,
		cum_size_literal(
		"\twhile (ends_counter <= end_i && ends->data[end_i-ends_counter] >= end-counter) {\n"
		"\t	if (ends->data[end_i-ends_counter] == end-counter) {\n"
		"\t		push_back_vector(size_t, &matches.ends, end-counter);\n"
		"\t		ends->data[end_i-ends_counter] = ~0;\n"
		"\t	}\n"
		"\t	++ends_counter;\n"
		"\t}\n"));

	cat_bytes_string_t(&reversedfa_options.final_node_action_code,
		cum_size_literal(
		"\tpush_back_vector(size_t, &matches.begins, end-counter);\n"));

	cat_bytes_string_t(&reversedfa_options.node_action_code,
		cum_size_literal(
		"\tif (counter == end) {\n"
		"\t	return matches;\n"
		"\t}\n"
		"\n"
		"\t++counter;\n"
		"\tc = buffer[end-counter];\n"));

	cat_bytes_string_t(&reversedfa_options.node_default_transition_code,
		cum_size_literal(
		"\t\treturn matches;\n"));

	/*
	vector_type(pnfa_node_t) searchdfa_nodes = 
		dfs_with_action_nfa(searchmdfa, dummy_print_nfa_node);
	unset_flags(&searchdfa_nodes, ~flag_nfa_node_visited);
	*/
	automata_from_options_goto_coder(
		searchmdfa_nodes,
		automata_from_options_node_goto_coder,
		&code,
		&searchdfa_options);
	/*
	vector_type(pnfa_node_t) reversedfa_nodes = 
		dfs_with_action_nfa(reversedfa, dummy_print_nfa_node);
	unset_flags(&reversedfa_nodes, ~flag_nfa_node_visited);
	*/
	automata_from_options_goto_coder(
		reversedfa_nodes,
		automata_from_options_node_goto_coder,
		&code,
		&reversedfa_options);


	cat_bytes_string_t(&code,
		cum_size_literal(
		"string_t read_file (const char *filename) {\n"
		"	string_t content;\n"
		"	place_string_t(&content);\n"
		"\n"
		"	FILE *file	= fopen(filename, \"rb\");\n"
		"	if (!file) {\n"
		"		perror(\"No file\\n\");\n"
		"		exit(EXIT_FAILURE);\n"
		"	}\n"
		"\n"
		"	fseek(file, 0, SEEK_END);\n"
		"	size_t length = ftell(file);\n"
		"	fseek(file, 0, SEEK_SET);\n"
		"\n"
		"	content.data = malloc(length+1);\n"
		"	if (!content.data)\n"
		"	{\n"
		"		perror(\"Memory shortage\\n\");\n"
		"		fclose(file);\n"
		"		exit(EXIT_FAILURE);\n"
		"	}\n"
		"	content.end = length;\n"
		"\n"
		"	fread(content.data, length, 1, file);\n"
		"	fclose(file);\n"
		"\n"
		"	return content;\n"
		"}\n"
		"\n"
		"void print_matches (matches_t *matches) {\n"
		"	printf(\"\\nbegins: [\");\n"
		"	forrall (&matches->begins, i) {\n"
		"		printf(\"%lu, \", matches->begins.data[i]);\n"
		"	}\n"
		"	printf(\"]\\nends: [\");\n"
		"	forrall (&matches->ends, i) {\n"
		"		printf(\"%lu, \", matches->ends.data[i]);\n"
		"	}\n"
		"	printf(\"]\\n\");\n"
		"}\n"
		"\n"
		"\n"
		"int main(int argc, char **argv) {\n"
		"	if (argc<2) {\n"
		"		perror(\"No filename\\n\");\n"
		"		exit(EXIT_FAILURE);\n"
		"	}\n"
		"\n"
		"	string_t content = read_file(argv[1]);\n"
		"\n"
		"	vector_type(size_t) ends = \n"
		"		 search_automata(content.data, content.end);\n"
		"\n"
		"	vector_type(matches_t) all_matches = vector(matches_t, 0);\n"
		"\n"
		"	forrall (&ends, i) {\n"
		"		if (~ends.data[i]) {\n"
		"			push_back_vector(\n"
		"				matches_t,\n"
		"				&all_matches,\n"
		"				reverse_automata(\n"
		"					content.data,\n"
		"					&ends,\n"
		"					i));\n"
		"		}\n"
		"	}\n"
		"\n"
		"	forrall (&all_matches, i) {\n"
		"		print_matches(&all_matches.data[i]);\n"
		"		destroy_matches_t(&all_matches.data[i]);\n"
		"	}\n"
		"	destroy_vector(matches_t, &all_matches);\n"
		"	destroy_vector(size_t, &ends);\n"
		"	destroy_string_t(&content);\n"
		"}\n"));
	/*
	destroy_vector(pnfa_node_t, &searchdfa_nodes);
	destroy_vector(pnfa_node_t, &reversedfa_nodes);
	*/
	destroy_goto_coder_options_t(&searchdfa_options);
	destroy_goto_coder_options_t(&reversedfa_options);

	return code;
}

