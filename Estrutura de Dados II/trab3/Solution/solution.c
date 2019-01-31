/*
 * Aluno: Vinicius Ferraço Arruda e Guilherme Sfalsin Scopel 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com e scopel.18@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 10 de outubro de 2015
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Job/job.h"
#include "solution.h"


/* Macro function to allocate memory on heap.  */
#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Out of memory.\n");exit(EXIT_FAILURE);}


Solution* newSolution(unsigned int n)
{
	Solution* new;

	get_memory(new, sizeof(Solution));
	get_memory(new->schedule, n * sizeof(unsigned int));
	new->cost = (unsigned int) -1;

	return new;
}


void setSolution(Solution* solution, unsigned int n, unsigned int* schedule, unsigned int i, Job** toRun, unsigned int except)
{
	unsigned int k;

	memcpy(solution->schedule, schedule, (i + 1) * sizeof(unsigned int));
	solution->schedule[i + 1] = except;

	for(k = i + 2; k < n; k++, toRun++)
	{
		if((*toRun)->index == except)
			toRun++;

		solution->schedule[k] = (*toRun)->index;
	}
}


void setInitialSolution(Solution* solution, unsigned int n, Job** toRun, unsigned int except)
{
	unsigned int k;

	solution->schedule[0] = except;

	for(k = 1; k < n; k++, toRun++)
	{
		if((*toRun)->index == except)
			toRun++;

		solution->schedule[k] = (*toRun)->index;
	}
}


void freeSolution(Solution* solution)
{
	free(solution->schedule);
	free(solution);
}


/**
 * printSolution: Imprime o custo do sequenciamento encontrado e o próprio sequenciamento de Jobs.
 * n: Número de jobs. 
 * cost: Custo do sequenciamento de Jobs.
 * schedule: Sequeciamento de Jobs a ser executado.
 */
void printSolution(Solution* solution, unsigned int n)
{
	unsigned int i;

	fprintf(stdout, "%u:", solution->cost);
	for(i = 0; i < n; i++)
	{
		fprintf(stdout, " %u", solution->schedule[i]);
	}
}

