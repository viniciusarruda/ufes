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
#include "list.h"


/* Macro function to allocate memory on heap.  */
#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Out of memory.\n");exit(EXIT_FAILURE);}


List* newNode(void* info)
{
	List* new;
	
	get_memory(new, sizeof(List));

	new->info = info;
	new->next = NULL;

	return new;
}


List* newList(void)
{
	return NULL;
}


void addInOrder(List** list, void* info, int (* cmp) (void *, void *))
{
	List* index;
	List* prev;
	List* new;

	if(*list == NULL)
	{
		*list = newNode(info);
		return;
	}

	prev = NULL;
	index = *list;
	new = newNode(info);

	do
	{
		if(cmp(info, index->info) <= 0)
		{
			if(prev == NULL)
				*list = new;
			else
				prev->next = new;

			new->next = index;
			return;
		}

		prev = index;
		index = index->next;
	}
	while(index != NULL);

	prev->next = new;
}


void justNFirst(List* list, unsigned int n, void (* freeInfo) (void *))
{
	List* prev = NULL;
	List* index = list;

	while(index != NULL && n != 0)
	{
		n--;
		prev = index;
		index = index->next;
	}

	if(index != NULL && prev != NULL)
	{
		freeList(index, freeInfo);
		prev->next = NULL;
	}
}


void freeList(List* list, void (* freeInfo) (void *))
{
	List* tmp;

	while(list != NULL)
	{	
		tmp = list;
		list = list->next;
		freeInfo(tmp->info);
		free(tmp);
	}
}


void* removeFirst(List** list)
{
	List* tmp;
	void* ret;

	if(*list == NULL)
		return NULL;


	tmp = *list;
	*list = tmp->next;

	ret = tmp->info;
	free(tmp);

	return ret;
}


