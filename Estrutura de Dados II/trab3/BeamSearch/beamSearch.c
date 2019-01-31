
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
#include "beamSearch.h"
#include "../BranchBound/branchBound.h"
#include "../BList/blist.h"
#include "../Solution/solution.h"


/* Macro function to allocate memory on heap.  */
#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Out of memory.\n");exit(EXIT_FAILURE);}

static void addChildren(BList* list, Solution* solution, Bound* father, unsigned int n);
static void newStart(BList* list, Solution* solution, Job** jobs, unsigned int n);


static void newStart(BList* list, Solution* solution, Job** jobs, unsigned int n)
{
	unsigned int i, lb, ub, t, cost;
	Bound* bound;

	for(i = 0; i < n; i++)
	{
		t = jobs[i]->life;

		lb = ub = cost = (t > jobs[i]->deadline) ? jobs[i]->penalty : 0;

		if(n > 1)
			calcBound(jobs, n, t, &lb, &ub, i);


		if(lb <= solution->cost)
		{
			if(ub < solution->cost)
				solution->cost = ub;

			if(lb == ub)
			{
				if(ub == solution->cost)
					setInitialSolution(solution, n, jobs, jobs[i]->index);
			}
			else 
			{
				bound = newBound(t, cost, lb, ub, 0, NULL, newToRun(jobs, n, jobs[i]->index));
				get_memory(bound->schedule, sizeof(unsigned int));
				bound->schedule[0] = jobs[i]->index;
				addInOrderB(list, bound);
			}
		}
	}
}


static void addChildren(BList* list, Solution* solution, Bound* father, unsigned int n)
{
	unsigned int i, lb, ub, cost, t;
	Job** jobs;

	jobs = father->toRun;

	n = n - (1 + father->i_s);

	for(i = 0; i < n; i++)
	{
		t = jobs[i]->life + father->t;

		lb = ub = cost = (t > jobs[i]->deadline) ? jobs[i]->penalty + father->cost : father->cost;

		if(n > 1)
			calcBound(jobs, n, t, &lb, &ub, i);

		if(lb <= solution->cost)
		{
			if(ub < solution->cost)
				solution->cost = ub;

			if(lb == ub)
			{
				if(ub == solution->cost)
					setSolution(solution, n + 1 + father->i_s, father->schedule, father->i_s, jobs, jobs[i]->index);
			}
			else
			{
				addInOrderB(list, newBound(t, cost, lb, ub, father->i_s + 1, newSchedule(father->schedule, father->i_s, jobs[i]->index), newToRun(jobs, n, jobs[i]->index)));
			}
		}
	}
}


void beamSearchDepth(Job** jobs, unsigned int n, Solution* solution, unsigned int beam_width)
{	
	Bound* bound;
	BList* list = newBList(beam_width, compareUpperBound, freeBound);

	newStart(list, solution, jobs, n);

	while(!isEmpty(list))
	{
		bound = (Bound *) removeBFirst(list);
		addChildren(list, solution, bound, n);
		freeBound(bound);
	}

	freeBList(list);
}


void beamSearchBreadth(Job** jobs, unsigned int n, Solution* solution, unsigned int beam_width)
{
	Bound* bound;
	BList* prev = newBList(beam_width, compareUpperBound, freeBound);
	BList* next = duplicateBList(prev);

	newStart(prev, solution, jobs, n);

	do
	{
		while(!isEmpty(prev))
		{
			bound = (Bound *) removeBFirst(prev);
			addChildren(next, solution, bound, n);
			freeBound(bound);
		}

		removeBList(prev, next);
	}
	while(!isEmpty(prev));

	freeBList(prev);
	freeBList(next);
}
