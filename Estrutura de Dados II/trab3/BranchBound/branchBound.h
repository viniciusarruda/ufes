/*
 * Aluno: Vinicius Ferraço Arruda e Guilherme Sfalsin Scopel 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com e scopel.18@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 10 de outubro de 2015
 */

#ifndef BB_H
#define BB_H

#include "../List/list.h"
#include "../Job/job.h"
#include "../Solution/solution.h"

typedef struct bound
{
	unsigned int t;
	unsigned int cost;
	unsigned int lb;
	unsigned int ub;
	unsigned int i_s;
	unsigned int* schedule; 
	Job** toRun;
} Bound;

Bound* newBound(unsigned int t, unsigned int cost, unsigned int lb, unsigned int ub, unsigned int i_s, unsigned int* schedule, Job** toRun);
unsigned int* newSchedule(unsigned int* schedule, unsigned int i, unsigned int index);
Job** newToRun(Job** oldToRun, unsigned int n, unsigned int except);
void calcBound(Job** jobs, unsigned int n, unsigned int t, unsigned int* lb, unsigned int* ub, unsigned int index);
void freeBound(void* info);
void branchBound(Job** jobs, unsigned int n, Solution* solution);
int compareUpperBound(void* a, void* b);

#endif

