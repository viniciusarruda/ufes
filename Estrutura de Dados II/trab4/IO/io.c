/*
 * Aluno: Vinicius Ferraço Arruda 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com 
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 27 de novembro de 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include "io.h"


#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Out of memory.\n");exit(EXIT_FAILURE);}


Number* readNumbersList(FILE* stream, unsigned int n)
{
	unsigned int i;
	Number* new;
	Number* index;

	get_memory(new, sizeof(Number));
	fscanf(stream, "%u", &new->value);
	index = new;

	for(i = 1; i < n; i++)
	{
		get_memory(index->next, sizeof(Number));
		index = index->next;
		fscanf(stream, "%u", &index->value);
	}

	index->next = NULL;

	return new;
}


void freeNumbersList(Number* numbers)
{
	Number* tmp;

	while(numbers != NULL)
	{
		tmp = numbers;
		numbers = numbers->next;
		free(tmp);
	}
}


void printNumbersList(FILE* stream, Number* numbers)
{
	while(numbers != NULL)
	{
		fprintf(stream, "%u\n", numbers->value);
		numbers = numbers->next;
	}
}


unsigned int* readNumbers(FILE* stream, unsigned int n)
{
	unsigned int i;
	unsigned int* numbers = (unsigned int *) malloc(n * sizeof(unsigned int));	

	if(numbers == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < n; i++)
	{
		fscanf(stream, "%u", &numbers[i]);
	}

	return numbers;
}


void printNumbers(FILE* stream, unsigned int* numbers, unsigned int n)
{
	unsigned int i;

	for(i = 0; i < n; i++)
	{
		fprintf(stream, "%u\n", numbers[i]);
	}
}

