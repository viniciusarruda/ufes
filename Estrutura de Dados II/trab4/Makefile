CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -pedantic  
GERA = gera.o
SORT = trab4.o Sort/sort.o Sort/radix.o IO/io.o
LIBS =  

all: trab4 gera documentacao

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 
	
gera: $(GERA)
	$(CC) -o $@ $^ $(LIBS) 
	$(MAKE) cleanGera

trab4: $(SORT)
	$(CC) -o $@ $^ $(LIBS) 
	$(MAKE) cleanSort

documentacao: 
	pdflatex -jobname trab4 artigo.tex
	pdflatex -jobname trab4 artigo.tex

cleanGera:
	rm -f *.o

cleanSort:
	rm -f *.o
	rm -f Sort/*.o
	rm -f IO/*.o
