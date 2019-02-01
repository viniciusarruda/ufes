
# Author: Vinicius Ferraco Arruda
# Email:  viniciusferracoarruda@gmail.com
# Professor: Eduardo Zambon
# Subject: Compilers
# Assignment: Implementation of an ast and table of symbols and literals for the language C-Minus

CC = gcc
LEX = flex
YACC = bison
#CFLAGS = -Wall -Wextra -Wconversion -pedantic
CFLAGS = -Wall -Wextra -g -ggdb
TRAB = src/ast/ast.o src/util/list.o src/parser.o src/scanner.o src/table/table.o src/interpreter/stack.o src/interpreter/interpreter.o
LIBS = -lfl

all: trab4

debug: 
	$(MAKE) all DEFINE=-DDEBUG_STACK

%.c: %.y
	$(YACC) -o $@ $<

%.c: %.l
	$(LEX) -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINE)

trab4: $(TRAB)
	$(CC) -o $@ $^ $(LIBS)
	$(MAKE) clean

clean:
	rm -f src/*.o
	rm -f src/ast/*.o
	rm -f src/util/*.o
	rm -f src/table/*.o
	rm -f src/parser.h
	rm -f src/interpreter/*.o

