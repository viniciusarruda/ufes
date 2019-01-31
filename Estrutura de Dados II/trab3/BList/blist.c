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
#include "blist.h"


/* Macro function to allocate memory on heap.  */
#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Out of memory.\n");exit(EXIT_FAILURE);}

static Node* newNode(void* info);

static Node* newNode(void* info)
{
	Node* new;
	
	get_memory(new, sizeof(Node));

	new->info = info;
	new->next = NULL;
	new->prev = NULL;

	return new;
}


BList* newBList(unsigned int max, int (* cmp) (void *, void *), void (* freeInfo) (void *))
{
	BList* new;

	get_memory(new, sizeof(BList));

	new->n = 0;
	new->max = max;
	new->cmp = cmp;
	new->freeInfo = freeInfo;
	new->first = NULL;
	new->last = NULL;

	return new;
}


BList* duplicateBList(BList* list)
{
	BList* new;

	get_memory(new, sizeof(BList));

	new->n = list->n;
	new->max = list->max;
	new->cmp = list->cmp;
	new->freeInfo = list->freeInfo;
	new->first = list->first;
	new->last = list->last;

	return new;
}


void removeBList(BList* to, BList* from)
{
	to->n = from->n;
	to->first = from->first;
	to->last = from->last;
	from->n = 0;
	from->first = NULL;
	from->last = NULL;
}


void addInOrderB(BList* list, void* info)
{
	Node* index;
	Node* new;
	unsigned int i;

	if(list->first == NULL)
	{
		list->first = list->last = newNode(info);
		list->n++;
		return;
	}

	i = 0;
	index = list->first;
	
	do
	{
		if(list->cmp(info, index->info) <= 0)
		{
			new = newNode(info);

			if(index->prev == NULL)
			{
				list->first = new;
			}
			else
			{
				index->prev->next = new;
				new->prev = index->prev;
			}

			index->prev = new;
			new->next = index;

			if(list->n == list->max)
			{
				index = list->last;
				list->last = index->prev;
				list->last->next = NULL;
				list->freeInfo(index->info);
				free(index);
			}
			else
			{
				list->n++;
			}

			return;
		}

		i++;
		index = index->next;
	}
	while(index != NULL && i < list->max);

	if(index == NULL && i < list->max)
	{	
		new = newNode(info);
		new->prev = list->last;
		list->last->next = new;
		list->last = new;
		list->n++;
	}
	else
	{
		list->freeInfo(info);
	}
}


void freeBList(BList* list)
{
	Node* tmp;

	while(list->first != NULL)
	{	
		tmp = list->first;
		list->first = list->first->next;
		list->freeInfo(tmp->info);
		free(tmp);
	}

	free(list);
}


void* removeBFirst(BList* list)
{
	Node* tmp;
	void* info;

	if(list->first == NULL)
		return NULL;

	list->n--;
	tmp = list->first;
	list->first = tmp->next;
	
	if(list->n == 0)
		list->last = NULL;
	else
		list->first->prev = NULL;
	

	info = tmp->info;
	
	free(tmp);
	
	return info;
}
