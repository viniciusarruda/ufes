CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -pedantic  
OBJ = trab3.o Job/job.o BranchBound/branchBound.o BeamSearch/beamSearch.o Solution/solution.o List/list.o BList/blist.o
LIBS =  

all: trab3 documentacao

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 
	
trab3: $(OBJ)
	$(CC) -o $@ $^ $(LIBS) 
	$(MAKE) clean

documentacao: 
	pdflatex documentacao.tex
	pdflatex documentacao.tex

clean:
	rm -f *.o
	rm -f Job/*.o
	rm -f BranchBound/*.o
	rm -f BeamSearch/*.o
	rm -f List/*.o
	rm -f BList/*.o
	rm -f Solution/*.o
