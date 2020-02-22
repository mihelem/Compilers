#ifndef PARSER_HELPER_HEADER
#define PARSER_HELPER_HEADER

#include <stdint.h>
#include "scangen_types.h"

// (memory unsafe)
void set_options_from_command_line(int argc, char **argv);
void print_response (nfa_t nfa[static 1]);

#endif