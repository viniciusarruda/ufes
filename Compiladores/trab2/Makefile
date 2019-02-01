# Author: Vinicius Ferraco Arruda
# Email:  viniciusferracoarruda@gmail.com
# Professor: Eduardo Zambon
# Subject: Compilers
# Assignment: Implementation of a parser for the language C-Minus

CC = gcc
LEX = flex
YACC = bison
#CFLAGS = -Wall -Wextra -Wconversion -pedantic
CFLAGS = -Wall -Wextra 
TRAB = parser.o scanner.o
LIBS = -lfl

all: trab2

%.c: %.y
	$(YACC) -o $@ $<

%.c: %.l
	$(LEX) -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 

trab2: $(TRAB)
	$(CC) -o $@ $^ $(LIBS)
	$(MAKE) clean

clean:
	rm -f *.o
	rm -f parser.h

