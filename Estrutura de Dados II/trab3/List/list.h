/*
 * Aluno: Vinicius Ferraço Arruda e Guilherme Sfalsin Scopel 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com e scopel.18@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 10 de outubro de 2015
 */

#ifndef LIST_H
#define LIST_H


typedef struct list
{
	void* info;
	struct list* next;
} List;


List* newList(void);
List* newNode(void* info);
void addInOrder(List** list, void* info, int (* cmp) (void *, void *));
void justNFirst(List* list, unsigned int n, void (* freeInfo) (void *));
void freeList(List* list, void (* freeInfo) (void *));
void* removeFirst(List** list);


#endif

