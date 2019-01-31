/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 12 de outubro de 2015
 */

#ifndef BTREE_H               
#define BTREE_H        

/*
 * MIN: Quantidade mínima de chaves em cada nó da árvore (exceto para o nó raiz que é 1). 
 * MAX: Quantidade máxima de chaves em cada nó da árvore. 
 * ATENÇÃO: Esta implementação de árvore B segue a seguinte regra: MAX deve ser exatamente duas vezes MIN mais 1.
 */
#define MIN 50 
#define MAX 101  /* (MAX = 2 * MIN + 1) */


/*
 * Estrutura de cada nó da árvore B.
 */
struct btree
{
	int leaf;                              /* Flag indicando se o nó é uma folha ou não. */
	size_t n;                              /* Número de chaves atual do nó. */
	void* keys[MAX];                       /* Ponteiros para as chaves (informação da árvore). */
	struct btree* children[MAX + 1];       /* Ponteiros para os nós filhos. */
};

typedef struct btree Btree;



Btree* new(void);
void insert(Btree** root, void* key, int (* cmp) (void *, void *));
void freeBtree(Btree* btree, void (* freeKey) (void *));
void* binarySearch(Btree* btree, void* key, int (* cmp) (void *, void *));
Btree* mountBtree(FILE* stream, void* (* mountKey) (FILE *));
void flushBtree(FILE* stream, Btree* btree, void (* flushKey) (FILE *, void *));
void convertBtree(Btree* btree, void* data, void (* insert) (void *, void *));


#endif
