/*
 * Aluno: Vinicius Ferraço Arruda 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com 
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 27 de novembro de 2015
 */
 
/* http://athlan.pl/incopy/polsl-benchmark-sort-algorithms/Main.pdf */
/* http://rosettacode.org/wiki/Rosetta_Code                         */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Sort/sort.h"
#include "Sort/radix.h"
#include "IO/io.h"


static int map(const char* algorithm);


int main(int argc, char** argv)
{
	int mapped, n;
	unsigned int* numbers;
	Number* numbersList;
	void (* sort[])(unsigned int *, unsigned int) = {bubble, shake, insertion, shell, selection, rank, quickprimeiro, quickcentral, quickrandom, quickmediana3, merge, heap};
	void (* radixsort[])(Number **) = {radix, radixbin};

	/* Question: What is the most beautiful and elegant way to do a menu for a program in C ? */
	/* If you have some good ideas, email me: viniciusferracoarruda@gmail.com, Thanks !       */

	if(argc != 3)
	{
		fprintf(stdout, "Usage: %s algorithm n\n", argv[0]);
	}
	else if((n = atoi(argv[2])) <= 0)
	{
		fprintf(stdout, "n cannot be negative or zero.\n");
	}
	else if((mapped = map(argv[1])) < 0)
	{
		fprintf(stdout, "Invalid algorithm. \n");
	}
	else if(mapped < 12)
	{
		numbers = readNumbers(stdin, (unsigned int) n);
		sort[mapped](numbers, (unsigned int) n);
		printNumbers(stdout, numbers, (unsigned int) n); 
		free(numbers);
	}
	else
	{
		numbersList = readNumbersList(stdin, (unsigned int) n);
		radixsort[mapped - 12](&numbersList);
		printNumbersList(stdout, numbersList); 
		freeNumbersList(numbersList);
	}

	return 0;
}


static int map(const char* algorithm) 
{
	int i;
	const char* functions[] = {"bubble", "shake", "insertion", "shell", "selection", "rank", "quickprimeiro", "quickcentral", "quickrandom", "quickmediana3", "merge", "heap", "radix", "radixbin"};

	for(i = 0; i < 14; i++)
		if(!strcmp(functions[i], algorithm))
			return i;

	return -1;
}


