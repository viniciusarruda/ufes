CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -pedantic  
OBJ = trab2.o Search/search.o Word/word.o Hash/hashEncadeada.o Hash/hashLinear.o Hash/rehash.o Hash/hashUtil.o GenericList/GenericList.o Btree/Btree.o 
LIBS =  

all: trab2 documentacao

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 
	
trab2: $(OBJ)
	$(CC) -o $@ $^ $(LIBS) 
	$(MAKE) clean

documentacao: 
	pdflatex documentacao.tex

clean:
	rm -f *.o
	rm -f GenericList/*.o
	rm -f Word/*.o
	rm -f Hash/*.o
	rm -f Search/*.o
	rm -f Btree/*.o

