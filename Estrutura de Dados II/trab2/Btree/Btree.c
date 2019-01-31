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
#include <string.h>
#include "Btree.h"
#include "../Word/word.h"


/*
 * Macros indicando se a pilha de recursão da insersão da árvore esta subindo (UP) em direção as folhas da árvore ou se 
 * está descendo (DOWN) em direção à raíz da árvore. 
 */
#define DOWN 0
#define UP   1


/*
 * Macros para identificar se um nó é folha (TRUE) ou não (FALSE).
 */
#define FALSE 0
#define TRUE  1


/*
 * Macro para alocar memória no heap de size bytes para o ponteiro ptr.
 */
#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Out of memory.\n");exit(EXIT_FAILURE);}


/*
 * Funções utilizadas apenas neste arquivo.
 */
static Btree* newBtree(void);
static void split(Btree* btree, void** promoted, Btree** left, Btree** right);
static int insert_key(Btree* btree, void** key, Btree** left, Btree** right, int* way, int (* cmp) (void *, void *));
static size_t getIndex(Btree* btree, void* key, int (* cmp) (void *, void *));


/*
 * Cria um nó Btree.
 */
static Btree* newBtree(void)
{
	Btree* btree;
	size_t i;
	
	get_memory(btree, sizeof(Btree));

	btree->n = 0;
	
	/* memset zeros nao seguro. */
	for(i = 0; i < MAX; i++)
	{
		btree->keys[i] = NULL;
		btree->children[i] = NULL;
	}
	btree->children[i] = NULL;

	return btree;
}


/*
 * Faz a divisão de um nó que atingiu o tamanho MAX em dois nós de tamanho MIN e uma chave central.
 */
static void split(Btree* btree, void** promoted, Btree** left, Btree** right)
{
	*left = newBtree();
	*right = newBtree();

	memcpy((*left)->keys, btree->keys, MIN * sizeof(void *));
	memcpy((*left)->children, btree->children, (MIN + 1) * sizeof(Btree *));

	*promoted = btree->keys[MIN];

	memcpy((*right)->keys, &btree->keys[MIN + 1], MIN * sizeof(void *));
	memcpy((*right)->children, &btree->children[MIN + 1], (MIN + 1) * sizeof(Btree *));

	(*left)->leaf = (*right)->leaf = btree->leaf;
	(*left)->n = (*right)->n = MIN;

	free(btree);
}


/*
 * Função recursiva que insere uma chave e seus filhos na árvore.
 * btree: A árvore B;
 * key: Chave;
 * left: Filho da esquerda;
 * right: Filho da direita;
 * way: Sentido em que a pilha de recursão cresce. 
 		Se way == UP, então tenta inserir key em um nó folha.
 		Se way == DOWN, então tenta inserir o nó key no nó atual.
 		Através desta flag se faz o controle de quando houver um split de um nó, a chave central que deve subir deve 
 		ser inserida em um nó superior ao da folha, por isso a flag way == DOWN (descendo da árvore em direção à raíz).
 * Retorna 1 caso houve split do nó e key deve ser inserida em um nó superior ao corrente ou 0 caso não houve split do nó.
 */
static int insert_key(Btree* btree, void** key, Btree** left, Btree** right, int* way, int (* cmp) (void *, void *))
{
	size_t i = getIndex(btree, *key, cmp);  

	if(btree->leaf == FALSE && *way == UP) 
	{
		if(insert_key(btree->children[i], key, left, right, way, cmp))
			return insert_key(btree, key, left, right, way, cmp);
	}
	else
	{
		*way = DOWN;

		if(i != btree->n)
		{
			memmove(&btree->keys[i + 1], &btree->keys[i], (btree->n - i) * sizeof(void *));

			if(i == 0)
				memmove(&btree->children[1], &btree->children[0], (btree->n + 1) * sizeof(Btree *));
			else
				memmove(&btree->children[i + 2], &btree->children[i + 1], (btree->n - i) * sizeof(Btree *));
		}

		btree->keys[i] = *key;
		btree->n++;

		btree->children[i] = *left;
		btree->children[i + 1] = *right;


		if(btree->n == MAX)
		{
			split(btree, key, left, right);
			return 1;
		}
	}

	return 0;
}


/*
 * Inicializa uma árvore B vazia.
 */
Btree* new(void)
{
	return NULL;
}


/*
 * Insere uma chave key na árvore B.
 */
void insert(Btree** root, void* key, int (* cmp) (void *, void *))
{
	Btree* left;
	Btree* right;
	int way;

	left = NULL;
	right = NULL;
	way = UP;


	if(*root == NULL)
	{
		*root = newBtree();
		(*root)->keys[0] = key;
		(*root)->children[0] = left;
		(*root)->children[1] = right;
		(*root)->n++;
		(*root)->leaf = TRUE;
	}
	else if(insert_key(*root, &key, &left, &right, &way, cmp)) 
	{
		*root = newBtree();
		(*root)->keys[0] = key;
		(*root)->children[0] = left;
		(*root)->children[1] = right;
		(*root)->n++;
		(*root)->leaf = FALSE;
	}
}


/*
 * Libera memória ocupada pela árvore B.
 */
void freeBtree(Btree* btree, void (* freeKey) (void *))
{
	size_t i;
	
	if(btree != NULL)
	{
		for(i = 0; i < btree->n + 1; i++)
		{
			freeBtree(btree->children[i], freeKey);
		}

		for(i = 0; i < btree->n; i++)
		{
			freeKey(btree->keys[i]);
		}

		free(btree);
	}
}


/*
 * Faz a busca na árvoreB pela chave key, se ela estiver na árvore, a informação contendo key é retornada e NULL caso contrário.
 */
void* binarySearch(Btree* btree, void* key, int (* cmp) (void *, void *))
{
	long int inf = 0; /* Não pode ser size_t aqui, pois inf e sup podem assumir valores negativos. */
	long int sup;  
	long int meio;
	size_t index;


	if(btree == NULL)
	{
		return NULL;
	}

	sup = (long int) btree->n - 1;  

	while (inf <= sup) 
	{
		meio = (inf + sup) / 2;

		if(cmp(key, btree->keys[meio]) == 0)
		{
			return btree->keys[meio];                       /* Retorna o endereço da informação. */
		}
		else if(cmp(key, btree->keys[meio]) < 0)
		{
			if(meio == 0 || cmp(key, btree->keys[meio - 1]) > 0)  
			{
		        index = (size_t) meio;
			}

			sup = meio - 1;
		}
		else
		{
			if(meio == (long int) btree->n - 1)
			{
		        index = btree->n;
			}
		   
			inf = meio + 1;
		}
	}

	return binarySearch(btree->children[index], key, cmp); 
}


/*
 * Retorna o indice do nó btree de onde a chave key deve ser inserida. 
 */
static size_t getIndex(Btree* btree, void* key, int (* cmp) (void *, void *))
{
     size_t inf = 0; 
     size_t sup = btree->n - 1;  
     size_t meio;

     while (inf <= sup) 
     {
          meio = (inf + sup) / 2;

          if(cmp(key, btree->keys[meio]) == 0)
          {
               break;   /* Este caso nunca irá ocorrer, pois não poderá existir chaves duplicadas na árvore. */
          }             /* A função handle_word em word.c trata isto para não ocorrer duplicação.            */         
          else if(cmp(key, btree->keys[meio]) < 0)   
          {
          	if(meio == 0 || cmp(key, btree->keys[meio - 1]) > 0)  
          		return meio;
          	else
            	sup = meio - 1;
          }
          else
          {
          	if(meio == btree->n - 1)
          		return btree->n;
          	else
               inf = meio + 1;
          }
     }

     return meio; 
}


/*
 * Monta a árvore B do arquivo.
 */
Btree* mountBtree(FILE* stream, void* (* mountKey) (FILE *))
{
	Btree* btree; 
	size_t size, i;

	fscanf(stream, "%lu", &size);
	fgetc(stream); /* skips \n */
	
	if(size == 0)
	{
		return NULL;
	}

	btree = newBtree();

	btree->n = size;

	fscanf(stream, "%d", &btree->leaf);
	fgetc(stream); /* skips \n */


	for(i = 0; i < btree->n; i++)
	{
		btree->keys[i] = mountKey(stream);
	}

	fgetc(stream); /* skips \n */


	if(btree->leaf == FALSE)
	{
		for(i = 0; i < btree->n + 1; i++)
		{
			btree->children[i] = mountBtree(stream, mountKey);
		}
	}

	return btree;
}


/*
 * Armazena a árvore B no arquivo.
 */
void flushBtree(FILE* stream, Btree* btree, void (* flushKey) (FILE *, void *))
{
	size_t i;

	fprintf(stream, "%lu\n", btree->n);
	fprintf(stream, "%d\n", btree->leaf);

	for(i = 0; i < btree->n; i++)
	{
		flushKey(stream, btree->keys[i]);
	}

	fprintf(stream, "\n");

	if(btree->leaf == FALSE)
	{
		for(i = 0; i < btree->n + 1; i++)
		{
			flushBtree(stream, btree->children[i], flushKey);
		}
	}

	free(btree);
}


/*
 * Converte os dados da árvore B em uma outra estrutura de dados.
 * btree: A árvore B que contém os dados.
 * data: A nova estrutura de dados.
 * insert: Função que insere uma informação na estrutura de dados data.
 */
void convertBtree(Btree* btree, void* data, void (* insert) (void *, void *))
{
	size_t i;

	for(i = 0; i < btree->n; i++)
	{
		insert(data, btree->keys[i]);
	}

	if(btree->leaf == FALSE)
	{
		for(i = 0; i < btree->n + 1; i++)
		{
			convertBtree(btree->children[i], data, insert);
		}
	}

	free(btree);
}


