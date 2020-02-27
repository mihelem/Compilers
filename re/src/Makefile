LEX?=flex
YACC?=bison
YFLAGS?=-d -v
CFLAGS?=-O3 -Wall

BINDIR = /var/www/bin/
INCLUDEDIR = /var/www/include/
LIBDIR = /var/www/lib/

SCANGEN = $(BINDIR)scangen

all: $(SCANGEN)

scanner.o: parser.c

$(SCANGEN): scanner.o parser.o commons.o range.o nfa.o trie.o dfazer.o nfa_coder.o string_t.o parser_helper.o automata_coder.o
	$(CC) -o $@ $^
		rm -f parser.c y.tab.h
		mv *.o $(LIBDIR)
		cp *.h $(INCLUDEDIR)

clean:
	cd $(LIBDIR)
	rm -f scanner.o parser.o commons.o range.o nfa.o trie.o dfazer.o nfa_coder.o string_t.o parser_helper.o automata_coder.o
	rm -f $(SCANGEN)
