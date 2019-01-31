/*
 * Aluno: Vinicius Ferraço Arruda e Guilherme Sfalsin Scopel 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com e scopel.18@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 10 de outubro de 2015
 */


#ifndef SOLUTION_H                   /* Caso não tenha definido os headers. */
#define SOLUTION_H                   /* Define os headers. */

#include <stdio.h>
#include "../Job/job.h"

typedef struct
{
	unsigned int cost;
	unsigned int* schedule;
} Solution;


Solution* newSolution(unsigned int n);
void setSolution(Solution* solution, unsigned int n, unsigned int* schedule, unsigned int i, Job** toRun, unsigned int except);
void setInitialSolution(Solution* solution, unsigned int n, Job** toRun, unsigned int except);
void freeSolution(Solution* solution);
void printSolution(Solution* solution, unsigned int n);

#endif
