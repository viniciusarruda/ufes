/*
 * Aluno: Vinicius Ferraço Arruda e Guilherme Sfalsin Scopel 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com e scopel.18@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 10 de outubro de 2015
 */


#ifndef JOB_H                   /* Caso não tenha definido os headers contido em job.h. */
#define JOB_H                   /* Define os headers contido em job.h. */

#include <stdio.h>

typedef struct
{
	unsigned int index;
	unsigned int life;   /* Tempo de processamento, tempo de vida. */
	unsigned int deadline;   /* Deadline. */
	unsigned int penalty;    /* Multa, penalidade. */
} Job;


Job** getJobs(FILE* stream, unsigned int n);
void freeJobs(Job** jobs, unsigned int n);

#endif 


