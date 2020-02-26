%{
/* Author: Michele Miccinesi 02022020 	*/
#include "scangen_types.h"
#include "y.tab.h"	

int yycolumn = 1;

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;	\
	yylloc.first_column = yycolumn;	\
	yylloc.last_column = yycolumn + yyleng - 1;	\
	yycolumn += yyleng;
%}

%option noyywrap
%x ESCAPED

%%

[\n\r]+ 		yycolumn = 1;
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
<ESCAPED>x[0-9a-fA-F]{1,2} 	yylval.character = strtol(yytext+1, NULL, 16); BEGIN(INITIAL); return LITERAL;
<ESCAPED>[0-7]{1,3}		yylval.character = strtol(yytext, NULL, 8); BEGIN(INITIAL); return LITERAL;
<ESCAPED>.		yylval.character = yytext[0]; BEGIN(INITIAL); return LITERAL;

%%

string_t read_file (const char *filename) {
	string_t content;
	place_string_t(&content);

	FILE *file	= fopen(filename, "rb");
	if (!file) {
		perror("No file\n");
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	size_t length = ftell(file);
	fseek(file, 0, SEEK_SET);

	content.data = malloc(length+1);
	if (!content.data)
	{
		perror("Memory shortage\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	content.end = length;

	fread(content.data, length, 1, file);
	fclose(file);

	return content;
}

bool parse (const char *filename) {
	if (filename == NULL || strcmp(filename, ".") == 0) {
		yyparse();
		return false;
	}

	string_t content = read_file(filename);
	cat_bytes_string_t(&content, 2, "\0");

	YY_BUFFER_STATE buffer = yy_scan_buffer(content.data, content.end-content.begin);
	yy_switch_to_buffer(buffer);

	yyparse();

	yy_delete_buffer(buffer);

	return true;
}
