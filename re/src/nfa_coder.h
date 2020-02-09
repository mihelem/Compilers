#ifndef nfa_coder_header
#define nfa_coder_header

#include "dfazer.h"
#include "string_t.h"

//vector_type(pnfa_node_t) empty_trans_closure (nfa_t *nfa_in, nfa_t *nfa_out)
//vector_type(pnfa_node_t) unroll_nfa (nfa_t *nfa, nfa_t *tree)

string_t *dfa_node_goto_coder (string_t *code, nfa_node_t *node, const char *input);
char *dfa_goto_coder (nfa_t *dfa, const char *name, const char *input);

#endif