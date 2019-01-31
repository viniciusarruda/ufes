/*
 * Aluno: Vinicius Ferraço Arruda e Guilherme Sfalsin Scopel 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com e scopel.18@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 10 de outubro de 2015
 */

#ifndef B_LIST_H
#define B_LIST_H


typedef struct node
{
	void* info;
	struct node* next;
	struct node* prev;
} Node;


typedef struct blist
{
	unsigned int n;
	unsigned int max;
	int (* cmp) (void *, void *);
	void (* freeInfo) (void *);
	Node* first;
	Node* last;
} BList;

#define isEmpty(list) (!(list->n))

BList* newBList(unsigned int max, int (* cmp) (void *, void *), void (* freeInfo) (void *));
void addInOrderB(BList* list, void* info);
void freeBList(BList* list);
void* removeBFirst(BList* list);
BList* duplicateBList(BList* list);
void removeBList(BList* to, BList* from);

#endif

