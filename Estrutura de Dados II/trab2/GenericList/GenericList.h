/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 12 de outubro de 2015
 */
 
#ifndef LIST_H                   /* Caso não tenha definido os headers contido em GenericList.h. */
#define LIST_H                   /* Define os headers contido em GenericList.h. */


struct list                      /* Estrutura de um nó do tipo List. */
{
	void* info;                  /* Ponteiro genério para a informação. */
	struct list* next;           /* Ponteiro para o próximo nó da lista. */
};

typedef struct list List;


int emptyList(List* l);
List* createEmptyList(void);
void insertBegList(List** l, void* info);
void freeList(List** l, void (* freeInfo) (void *));
List* createNode(void* info);
void* removeBegList(List** l);
void* searchInfo(List* l, void* info, int (* equals) (void *, void *));
void travelsList(List* l, void (* function) (void *));
List* copyList(List* l, void* (* copyInfo) (void *));
void insertEndList(List** l, List** end, void* info);
int isInList(List* l, void* info, int (* equals) (void *, void *));
void intersection(List** a, List* b, int (* equals) (void *, void *), void (* freeInfo) (void *));
void backpropagation(List** l, void* data, void (* function) (void *, void *));


#endif 


