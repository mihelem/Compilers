%{
/* Author: Michele Miccinesi 02022020 	*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "scangen_types.h"
#include "parser_helper.h"

int yylex();

void yyerror(const char *s, ...);

%}

%code requires {
  bool parse(const char *filename);
}

%define parse.error verbose
%locations

%union {
	uint8_t character;
	range_t range;
	nfa_t nfa; 
}

%token <character> LITERAL
%type <character> rliteral
%type <range> subset lrange range
%type <nfa> atom ppiece cpiece piece pieces regexp

%%

start :	regexp				{ 	print_response(&$1); 
								destroy_nfa_t(destroy_nodes_of_nfa(&$1)); }

regexp : pieces				{ 	$$ = $1; }
	|	regexp '|' pieces	{ 	unite_nfa(&$$, &$1, &$3); }
	| 	regexp '&' pieces	{ 	$$ = intersection_dfazing_nfa(set_ids_of_nfa(&$1), set_ids_of_nfa(&$3)); 
								destroy_nfa_t(destroy_nodes_of_nfa(&$1)); 
								destroy_nfa_t(destroy_nodes_of_nfa(&$3)); }

pieces : piece 				{ 	$$ = $1; }
	|	pieces piece 		{	concatenate_nfa(&$$, &$1, &$2); }

piece : cpiece				{	$$ = $1; }
	|	'^' cpiece			{	complement_nfa(&$2, &$$); } 

cpiece : ppiece				{	$$ = $1; }
	|	'>' ppiece 			{ 	$$ = $2; 
								apply_flags_to_nfa(&$$, flag_nfa_node_final, ~flag_nfa_node_visited); }
	|	ppiece '<'			{ 	$$ = $1; 
								apply_flags_to_nfa(&$$, flag_nfa_node_initial, ~flag_nfa_node_visited); }
	|	'>' ppiece '<'		{ 	$$ = $2; 
								apply_flags_to_nfa(&$$, flag_nfa_node_initial|flag_nfa_node_final, ~flag_nfa_node_visited); }

ppiece : atom '*'			{ $$ = $1; make_avoidable_nfa(make_repeatable_nfa(&$$)); }
	|	atom '+'			{ $$ = $1; make_repeatable_nfa(&$$); }
	|	atom '?'			{ $$ = $1; make_avoidable_nfa(&$$); }
	|	atom				{ $$ = $1; }

atom :	'(' regexp ')'		{ $$ = $2; }
	|	'[' '^' subset ']'	{ $$ = range_to_nfa(complement_range(&$3)); }
	|	'[' subset ']'		{ $$ = range_to_nfa(&$2); }
	|	LITERAL				{ $$ = literal_to_nfa($1); }

subset : ']' 				{ set_in_range(clear_range(&$$), ']'); }
	|	'-' 				{ set_in_range(clear_range(&$$), '-'); }
	|	']' lrange			{ $$ = $2; set_in_range(&$$, ']'); }
	|	'-' lrange			{ $$ = $2; set_in_range(&$$, '-'); }
	|	lrange				{ $$ = $1; }

lrange :	lrange range 	{ $$ = union_range(&$1, &$2); }
 	|	lrange rliteral		{ $$ = $1; set_in_range(&$$, $2); }
	|	range 				{ $$ = $1; }
	| 	rliteral			{ set_in_range(clear_range(&$$), $1); }

range : rliteral '-' rliteral 		{ 
										clear_range(&$$); 
										if ($1 <= $3) 
										{ 
											uint8_t c = $1; 
											do { set_in_range(&$$, c); } while (c++ < $3); 
										} 
									}

rliteral :	LITERAL	{ $$ = $1; }
	|	'+'			{ $$ = '+'; }
	|	'*'			{ $$ = '*'; }
	|	'?'			{ $$ = '?'; }
	|	'|'			{ $$ = '|'; }
	|	'('			{ $$ = '('; }
	|	')'			{ $$ = ')'; }
	|	'['			{ $$ = '['; }
	|	'&'			{ $$ = '&'; }
	|	'<'			{ $$ = '<'; }
	|	'>'			{ $$ = '>'; }

%%

void yyerror(const char *s, ...) {
	va_list ap;
	va_start(ap, s);

	printf("\n~~~~~~~~~~\n" "error" "\n");
	if (yylloc.first_line) {
		printf( 
			"%d.%d-%d.%d: error: ", 
			yylloc.first_line,
			yylloc.first_column,
			yylloc.last_line,
			yylloc.last_column);
	}
	vprintf(s, ap);
	printf("\n");
}

int main(int argc, char **argv) {
	set_options_from_command_line(argc, argv);

	char filename[5000];
	scanf("%s", filename);

	parse(filename);

	return 0;
}