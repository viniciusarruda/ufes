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
#include "job.h"


/* Macro function to allocate memory on heap.  */
#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Out of memory.\n");exit(EXIT_FAILURE);}


/**
 * getJobs: Lê de stream n jobs.
 * n: Número de jobs a ser lido de stream.
 * return: Retorna um array de jobs, no intervalo [0, n).
 */
Job** getJobs(FILE* stream, unsigned int n)
{
	unsigned int i;
	Job** jobs;

	jobs = (Job **) malloc(n * sizeof(Job *));

	if(jobs == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < n; i++)
	{
		get_memory(jobs[i], sizeof(Job));

		jobs[i]->index = i;

		if(fscanf(stream, "%u %u %u", &jobs[i]->life, &jobs[i]->deadline, &jobs[i]->penalty) != 3)
			fprintf(stderr, "SOMETHING IS GOING REALLY WRONG !\n");
	}

	return jobs;
}


void freeJobs(Job** jobs, unsigned int n)
{
	unsigned int i;

	for(i = 0; i < n; i++)
		free(jobs[i]);
	free(jobs);
}

