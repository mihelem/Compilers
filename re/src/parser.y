%{
/* Author: Michele Miccinesi 02022020 	*/
#include <stdio.h>
#include <string.h>
#include "scangen_types.h"
#include "parser_helper.h"

int yylex();

void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}

/*lines regexp EOL	{ $$ = $1; push_back_vector(nfa_t, &$$, $2); 
								//printf("lines : lines regexp EOL\n"); 
								print_nfa(&$2, my_node_printer); }
	| */
%}

%union {
	range_t range;
	uint8_t character;
	nfa_t nfa; 
	vector_nfa_t_t list;
}

%token EOL
%token <character> LITERAL
%type <character> rliteral
%type <range> subset lrange range
%type <nfa> atom ppiece cpiece piece pieces regexp
%type <list> lines

%%

lines :	regexp EOL			{ 	
								$$ = vector(nfa_t, 0); 
								push_back_vector(nfa_t, &$$, $1); 
								print_response(&$1); 
								destroy_nodes_of_nfa(&$1); 
								destroy_vector(nfa_t, &$$); }
	;

regexp : pieces				{ 	$$ = $1; }
	|	regexp '|' pieces	{ 	unite_nfa(&$$, &$1, &$3); }
	| 	regexp '&' pieces	{ 	$$ = intersection_dfazing_nfa(set_ids_of_nfa(&$1), set_ids_of_nfa(&$3)); 
								destroy_nfa_t(destroy_nodes_of_nfa(&$1)); 
								destroy_nfa_t(destroy_nodes_of_nfa(&$3)); }
	;

pieces : piece 				{ 	$$ = $1; }
	|	pieces piece 		{	concatenate_nfa(&$$, &$1, &$2); }
	;

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
	;

atom :	'(' regexp ')'		{ $$ = $2; }
	|	'[' '^' subset ']'	{ $$ = range_to_nfa(complement_range(&$3)); }
	|	'[' subset ']'		{ $$ = range_to_nfa(&$2); }
	|	LITERAL				{ $$ = literal_to_nfa($1); }
	;

subset : ']' 				{ set_in_range(clear_range(&$$), ']'); }
	|	'-' 				{ set_in_range(clear_range(&$$), '-'); }
	|	']' lrange			{ $$ = $2; set_in_range(&$$, ']'); }
	|	'-' lrange			{ $$ = $2; set_in_range(&$$, '-'); }
	|	lrange				{ $$ = $1; }
	;

lrange :	lrange range 	{ $$ = union_range(&$1, &$2); }
 	|	lrange rliteral		{ $$ = $1; set_in_range(&$$, $2); }
	|	range 				{ $$ = $1; }
	| 	rliteral			{ set_in_range(clear_range(&$$), $1); }
	;

range : rliteral '-' rliteral 		{ 
										clear_range(&$$); 
										if ($1 <= $3) 
										{ 
											uint8_t c = $1; 
											do { set_in_range(&$$, c); } while (c++ < $3); 
										} 
									}
	;

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
	;

%%

int main(int argc, char **argv) {
	set_options_from_command_line(argc, argv);

	yyparse();

	return 0;
}