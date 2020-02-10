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
%type <nfa> atom piece pieces regexp
%type <list> lines

%%

lines :	regexp EOL			{ $$ = vector(nfa_t, 0); push_back_vector(nfa_t, &$$, $1); print_response(&$1); }
	;

regexp : pieces				{ $$ = $1; }
	|	regexp '|' pieces	{ $$ = union_nfa(&$1, &$3); }
	;

pieces : piece 				{ $$ = $1; }
	|	pieces piece 		{ $$ = concatenation_nfa(&$1, &$2); }
	;

piece : atom '*'			{ $$ = $1; make_avoidable_nfa(make_repeatable_nfa(&$$)); }
	|	atom '+'			{ $$ = $1; make_repeatable_nfa(&$$); }
	|	atom '?'			{ $$ = $1; make_avoidable_nfa(&$$); }
	|	atom				{ $$ = $1; }
	;

atom :	'(' regexp ')'		{ $$ = $2; }
	|	'[' '^' subset ']'	{ $$ = range_to_nfa(complement_range(set_complemented_range(&$3))); }
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
	;

%%

int main(int argc, char **argv) {
	set_options_from_command_line(argc, argv);

	yyparse();

	return 0;
}