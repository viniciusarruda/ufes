CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -pedantic  
OBJ = Codigos/trab1.o Codigos/exato.o Codigos/nn.o Codigos/opt.o Codigos/hull.o Codigos/GenericList/GenericList.o Codigos/Stack/Stack.o
LIBS = -lm 

all: trab1 documentacao

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 
	
trab1: $(OBJ)
	$(CC) -o $@ $^ $(LIBS) 
	$(MAKE) clean
	
documentacao: 
	pdflatex documentacao.tex
	pdflatex documentacao.tex

clean:
	rm -f Codigos/*.o
	rm -f Codigos/GenericList/*.o
	rm -f Codigos/Stack/*.o

