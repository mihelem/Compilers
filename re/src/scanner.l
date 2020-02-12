%{
/* Author: Michele Miccinesi 02022020 	*/
#include "scangen_types.h"
#include "y.tab.h"	

%}

%option noyywrap

%x ESCAPED

%%

[\n\r]+ 		return EOL;
[\t\b\a\f]		;
[][?()|*+^&<>-]	return *yytext;
\\				BEGIN(ESCAPED);
.				yylval.character = yytext[0]; return LITERAL;

<ESCAPED>a		yylval.character = '\a'; BEGIN(INITIAL); return LITERAL;
<ESCAPED>b 		yylval.character = '\b'; BEGIN(INITIAL); return LITERAL;
<ESCAPED>f 		yylval.character = '\f'; BEGIN(INITIAL); return LITERAL;
<ESCAPED>n 		yylval.character = '\n'; BEGIN(INITIAL); return LITERAL;
<ESCAPED>r 		yylval.character = '\r'; BEGIN(INITIAL); return LITERAL;
<ESCAPED>t 		yylval.character = '\t'; BEGIN(INITIAL); return LITERAL;
<ESCAPED>\\		yylval.character = '\\'; BEGIN(INITIAL); return LITERAL;
<ESCAPED>x[0-9a-fA-F]+ 	yylval.character = strtol(yytext+1, NULL, 16); BEGIN(INITIAL); return LITERAL;
<ESCAPED>[0-7]{1,3}		yylval.character = strtol(yytext, NULL, 8); BEGIN(INITIAL); return LITERAL;
<ESCAPED>.		yylval.character = yytext[0]; BEGIN(INITIAL); return LITERAL;

%%
