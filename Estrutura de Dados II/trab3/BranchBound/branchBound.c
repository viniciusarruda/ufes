/*
 * Aluno: Vinicius Ferraço Arruda e Guilherme Sfalsin Scopel 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com e scopel.18@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 10 de outubro de 2015
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "branchBound.h"
#include "../List/list.h"
#include "../Solution/solution.h"

/* Macro function to allocate memory on heap.  */
#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Out of memory.\n");exit(EXIT_FAILURE);}

static List* newStart(Solution* solution, Job** jobs, unsigned int n);
static void addChildren(Solution* solution, List** list, Bound* father, unsigned int n);

Bound* newBound(unsigned int t, unsigned int cost, unsigned int lb, unsigned int ub, unsigned int i_s, unsigned int* schedule, Job** toRun)
{
	Bound* new;

	get_memory(new, sizeof(Bound));

	new->t = t;
	new->cost = cost;
	new->lb = lb;
	new->ub = ub;
	new->i_s = i_s;
	new->schedule = schedule;
	new->toRun = toRun;

	return new;
}


unsigned int* newSchedule(unsigned int* schedule, unsigned int i, unsigned int index)
{
	unsigned int* new;
	get_memory(new, (i + 2) * sizeof(unsigned int));	
	memcpy(new, schedule, (i + 1) * sizeof(unsigned int));
	new[i + 1] = index;

	return new;
}


Job** newToRun(Job** oldToRun, unsigned int n, unsigned int except)
{ 
	unsigned int i;
	Job** new;
	Job* tmp;


	get_memory(new, (n - 1) * sizeof(Job *));

	i = 0;

	while(i < n - 1)
	{
		if((*oldToRun)->index == except) /* Passa aqui uma vez */
			oldToRun++;

		tmp = *oldToRun;

		new[i] = tmp;

		i++;
		oldToRun++;
	}

	return new;
}


void calcBound(Job** jobs, unsigned int n, unsigned int t, unsigned int* lb, unsigned int* ub, unsigned int index)
{
	unsigned int i;

	for(i = 0; i < n; i++)
	{
		if(t > jobs[i]->deadline)
			*lb += jobs[i]->penalty; 

		*ub += jobs[i]->penalty; 
	}

	if(t > jobs[index]->deadline)
		*lb -= jobs[index]->penalty;

	*ub -= jobs[index]->penalty;
}


static List* newStart(Solution* solution, Job** jobs, unsigned int n)
{
	unsigned int i, lb, ub, t, cost;
	Bound* bound;
	List* list;

	list = newList();

	for(i = 0; i < n; i++)
	{
		t = jobs[i]->life;

		lb = ub = cost = (t > jobs[i]->deadline) ? jobs[i]->penalty : 0;

		if(n > 1)
			calcBound(jobs, n, t, &lb, &ub, i);


		if(lb < solution->cost)
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
				addInOrder(&list, bound, compareUpperBound);
			}
		}
	}

	return list;
}


static void addChildren(Solution* solution, List** list, Bound* father, unsigned int n)
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

		if(lb < solution->cost)
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
				addInOrder(list, newBound(t, cost, lb, ub, father->i_s + 1, newSchedule(father->schedule, father->i_s, jobs[i]->index), newToRun(jobs, n, jobs[i]->index)), compareUpperBound);
			}
		}
	}
}


void freeBound(void* info)
{
	Bound* bound = (Bound *) info;
	free(bound->schedule);
	free(bound->toRun);
	free(bound);
}


void branchBound(Job** jobs, unsigned int n, Solution* solution)
{	
	Bound* bound;
	List* list = newStart(solution, jobs, n);

	while(list != NULL)
	{
		bound = (Bound *) removeFirst(&list);
		addChildren(solution, &list, bound, n);
		freeBound(bound);
	}
}


int compareUpperBound(void* a, void* b)
{
	Bound* x = a;
	Bound* y = b;

	return (int) x->ub - (int) y->ub;
}

