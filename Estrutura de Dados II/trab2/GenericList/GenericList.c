/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 12 de outubro de 2015
 */
 
 
#include <stdio.h>
#include <stdlib.h>
#include "GenericList.h"


int emptyList(List* l)                                        /* Verifica se a lista l está vazia. */
{
	return l == NULL;
}


List* createEmptyList(void)                                   /* Cria uma lista vazia. */
{
	return NULL;
}


List* createNode(void* info)                                  /* Cria um nó do tipo List com a informação passada como parâmetro. */
{
	List* new = (List *) malloc(sizeof(List));                              
	
	if(new == NULL)
	{	
		fprintf(stderr, "\nError while allocating memory.\n\n");
		exit(EXIT_FAILURE);
	}
	
	new->info = info;  
	new->next = NULL;

	return new;
}


void insertBegList(List** l, void* info)                     /* Insere um nó com a informação info no início da lista. */
{
	List* new = createNode(info);                            /* Cria um nó com a informação info. */
	
	new->next = *l;                                          /* O nó recentemente, tem como próximo o primeiro elemento da lista *l, */
	*l = new;                                                /* Se a lista *l for vazia (NULL), então aponta para NULL. */
}                                                            /* Atualiza o ponteiro *l para o primeiro elemento da lista. */


void* removeBegList(List** l)                                /* Remove um nó do início da lista. */
{
	List* temp;                                  
	void* info;
	
	if(*l == NULL)                                           /* Caso a lista *l estiver vazia. */
		return NULL;						                 /* Retorna NULL. */
		
	info = (*l)->info;	                                     /* info armazana a informação contida no primeiro nó da lista. */
	temp = (*l)->next;                                       /* temp armazena temporariamente o endereço do próximo nó. */
	free(*l);                                                /* Libera a memória que o primeiro nó consumia. */
	*l = temp;                                               /* Atualiza o ponteiro da lista *l para o atual primeiro elemento da lista. */

	return info;                                             /* Retorna a informação que estava armazenada no primeiro nó. */
}


void freeList(List** l, void (* freeInfo) (void *))    /* Percorre a lista *l liberando todos os seus nós. */
{
	List* temp;

	while(*l != NULL)                          /* Enquanto a lista *l não estiver vazia. */
	{
		temp = (*l)->next;                     /* Armazena temporariamente o ponteiro para o próximo nó. */
		freeInfo((*l)->info);                  /* Chama a função passada como parâmetro para liberar a informação que o usuário do TAD criou. */
		free(*l);                              /* Libera o nó corrente. */
		*l = temp;                             /* *l é atualizado, apontando para o próximo nó. */
	}										   /* Quando a lista estiver vazia, *l apontará para vazio (NULL). */
}


void* searchInfo(List* l, void* info, int (* equals) (void *, void *)) 
{
	while(l != NULL && !equals(l->info, info)) l = l->next;
	
	if(l == NULL) /* Not found element. */
		return NULL;

	return l->info;
}


void travelsList(List* l, void (* function) (void *))   /* Percorre uma lista l e aplica a função de callback function para cada informação. */
{	
	for(; l != NULL; l = l->next)                       /* Percorre a lista até seu fim. */
	{
		function(l->info);                              /* Chama o callback, passando a referência para a informação. */
	}
}


List* copyList(List* l, void* (* copyInfo) (void *))  
{	
	List* end;
	List* new = createEmptyList();

	for(; l != NULL; l = l->next)                       
	{
		insertEndList(&new, &end, copyInfo(l->info));                             
	}

	return new;
}


void insertEndList(List** l, List** end, void* info)          
{
	List* new = createNode(info);                    

	if(*l == NULL)
		*l = new; 
	else
		(*end)->next = new;

	*end = new;
} 


/* Aplica a função function nos dados da lista de trás para frente. data é um parâmetro extra a ser passado à function. */
void backpropagation(List** l, void* data, void (* function) (void *, void *))
{
	if(*l == NULL)                       
		return;
	else
	{
		backpropagation(&(*l)->next, data, function);
		function((*l)->info, data);
		free(*l);
		*l = NULL;
	}				                                         
}


int isInList(List* l, void* info, int (* equals) (void *, void *))
{
	while(l != NULL)
	{
		if(equals(l->info, info))
			return 1;

		l = l->next;
	}
	return 0;
}


/*
 * Faz a interseção dos dados de duas listas.
 * Ex: A = 0 2 4 6
       B = 1 3 6 9 10 2
       Lista após a interseção de A e B = 6 2 
 */
void intersection(List** a, List* b, int (* equals) (void *, void *), void (* freeInfo) (void *))
{
	List* end;
	void* info;
	List* new = createEmptyList();

	while((info = removeBegList(a)) != NULL) 
	{
		if(isInList(b, info, equals))
		{
			insertEndList(&new, &end, info);
		}
		else
		{
			freeInfo(info);
		}
	}

	*a = new;
}


