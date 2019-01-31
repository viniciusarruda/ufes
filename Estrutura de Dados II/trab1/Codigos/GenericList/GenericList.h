/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professora: Mariella Berger
 * Data: 02 de Setembro de 2015
 */
 
 
#ifndef LIST_H                   /* Caso não tenha definido os headers contido em GenericList.h. */
#define LIST_H                   /* Define os headers contido em GenericList.h. */


struct list                      /* https://en.wikipedia.org/wiki/Opaque_pointer#C   Eu deveria colocar esta struct no .c ? */
{
	void* info;                 
	struct list* next;          
	struct list* prev;
};

typedef struct list List;


int emptyList(List* l);
List* createEmptyList(void);  
void* getInfo(List* node);         
void insertAfterNode(List* node, void* info);     
void insertBegList(List** l, void* info);
List* next(List* node);
void* removeNode(List** l, List* node);
void freeListandInfo(List** l, void (* freeInfo) (void *));
void freeList(List** l);

#endif 


