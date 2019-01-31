/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professora: Mariella Berger 
 * Data: 02 de Setembro de 2015
 */


#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"


static Stack* createElement(void* info);


static Stack* createElement(void* info)                                    /* Cria um elemento com a informação info. */
{
	Stack* new = (Stack *) malloc(sizeof(Stack)); 

	if(new == NULL)
	{
		fprintf(stderr, "Error while allocating memory.\n");
		exit(EXIT_FAILURE);
	}
	
	new->info = info;
	new->next = NULL;                                     
	
	return new;
}


Stack* createEmptyStack(void)                                              /* Cria uma pilha vazia. */
{
	return NULL;
}


int emptyStack(Stack* s)                                                   /* Verifica se a pilha está vazia */
{
	return s == NULL;
}


void* lookStackInfo(Stack* s, int element)                                 /* Retorna a informação que está na pilha na posição element. */
{
	if(element < 0)                                                        /* Ex: lookStackInfo(s, 0) retorna a informação do topo da pilha. */
	{
		return NULL;
	}
	
	while(s != NULL && element > 0)	
	{	
		element--;
		s = s->next;
	}
	
	if(s == NULL)
	{
		return NULL;
	}
	
	return s->info;
}


void push(Stack** s, void* info)                                           /* Empilha um elemento com a informação info. */
{
	Stack* new = createElement(info);
	
	new->next = *s;
	*s = new;
}


void* pop(Stack** s)                                                       /* Retira o elemento do topo da pilha e retorna sua informação. */
{
	Stack* tmp;
	void* info;
	
	if(*s == NULL)
		return NULL;
	
	info = (*s)->info;
	tmp = (*s)->next;
	free(*s);
	*s = tmp;
	
	return info;
}


void freeStackandInfo(Stack** s, void (* freeInfo)(void *))       /* Percorre a pilha liberando todos os seus elementos e suas informações. */
{
	Stack* aux;
	
	while(*s != NULL)
	{
		freeInfo((*s)->info);
		aux = (*s)->next;
		free(*s);
		*s = aux;
	}
}


void freeStack(Stack** s)                                                  /* Percorre a pilha liberando todos os seus elementos. */
{
	Stack* aux;
	
	while(*s != NULL)
	{
		aux = (*s)->next;
		free(*s);
		*s = aux;
	}
}


