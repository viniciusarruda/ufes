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
#include "GenericList.h"


static List* createNode(void* info);             


int emptyList(List* l)                                        /* Verifica se a lista l está vazia. */
{
	return l == NULL;
}


List* createEmptyList(void)                                   /* Cria uma lista vazia. */
{
	return NULL;
}


static List* createNode(void* info)                           /* Cria um nó do tipo List com a informação passada como parâmetro. */
{
	List* new = (List *) malloc(sizeof(List));                              
	
	if(new == NULL)
	{	
		fprintf(stderr, "\nError while allocating memory.\n\n");
		exit(EXIT_FAILURE);
	}
	
	new->info = info;  
	new->next = NULL;
	new->prev = NULL;

	return new;
}


void* getInfo(List* node)                                     /* Retorna um ponteiro para a informação. */
{
	if(node == NULL)
	{
		return NULL;
	}
	return node->info;
}


void insertAfterNode(List* node, void* info)                  /* Cria um nó com uma informação info e o insere após o nó node. */
{
	List* new = createNode(info);
	
	new->next = node->next;

	if(node->next != NULL)
	{
		node->next->prev = new; 
	}

	node->next = new;
	new->prev = node;
}


void insertBegList(List** l, void* info)                      /* Insere um nó com a informação info no início da lista. */
{
	List* new = createNode(info);                           
	
	new->next = *l;                                         
	*l = new;                                               

	if(new->next != NULL)
	{	
		new->next->prev = new;
	}	
}


List* next(List* node)                                        /* Retorna o próximo elemento da lista após o nó node. */
{
	return node->next;
}


void* removeNode(List** l, List* node)                        /* Remove o nó node da lista l e retorna sua informação. */
{
	void* info;
	
	if(node->prev == NULL)  
	{
		*l = node->next;	
	}
	else
	{
		node->prev->next = node->next;
	}
	
	if(node->next != NULL)
	{
		node->next->prev = node->prev;
	}
	
	info = node->info;
	
	free(node);
	
	return info;
}


void freeListandInfo(List** l, void (* freeInfo) (void *))    /* Percorre a lista *l liberando todos os seus nós e suas informações. */
{
	List* temp;

	while(*l != NULL)                         
	{
		temp = (*l)->next;                    
		freeInfo((*l)->info);                 
		free(*l);                             
		*l = temp;                            
	}										  
}


void freeList(List** l)                                       /* Percorre a lista *l liberando todos os seus nós. */
{
	List* temp;

	while(*l != NULL)             
	{
		temp = (*l)->next;          
		free(*l);                 
		*l = temp;                
	}								
}
	
	
