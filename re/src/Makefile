LEX?=flex
YACC?=bison
YFLAGS?=-d
CFLAGS?=-O3 -Wall

all: ../web/bin/scangen

scanner.o: parser.c

../web/bin/scangen: scanner.o parser.o commons.o range.o nfa.o trie.o dfazer.o nfa_coder.o string_t.o parser_helper.o
	$(CC) -o $@ $^
		rm -f scangen_types.o scanner.o parser_helper.o parser.o commons.o range.o nfa.o string_t.o trie.o dfazer.o nfa_coder.o parser.c y.tab.h

clean:
	rm -f ../web/bin/scangen
