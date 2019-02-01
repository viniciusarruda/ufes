# Author: Vinicius Ferraco Arruda
# Email:  viniciusferracoarruda@gmail.com
# Professor: Eduardo Zambon
# Subject: Compilers
# Assignment: Implementation of a scanner for the language C-Minus

CC = gcc
LEX = flex
#CFLAGS = -Wall -Wextra -Wconversion -pedantic
CFLAGS = -Wall -Wextra 
TRAB = trab1.o
LIBS = -lfl

all: trab1

%.c: %.l
	$(LEX) -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 

trab1: $(TRAB)
	$(CC) -o $@ $^ $(LIBS)
	$(MAKE) clean

clean:
	rm -f *.o

