/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professora: Mariella Berger 
 * Data: 02 de Setembro de 2015
 */
 

#ifndef STACK_H                                     /* Caso não tenha definido os headers contido em Stack.h. */
#define STACK_H                                     /* Define os headers contido em Stack.h. */


struct stack                            
{
	void* info;
	struct stack* next;
};

typedef struct stack Stack;


Stack* createEmptyStack(void);
int emptyStack(Stack* s);
void freeStack(Stack** stack);
void freeStackandInfo(Stack** s, void (* freeInfo)(void *));
void* lookStackInfo(Stack* s, int element);
void* pop(Stack** s);
void push(Stack** s, void* info);

#endif 


