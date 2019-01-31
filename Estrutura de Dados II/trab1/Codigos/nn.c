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

#include "GenericList/GenericList.h"


static List* createWhitelist(int n);


/*
 * Função que calcula um caminho para o problema do caixeiro viajante de maneira gulosa, passando pela cidade mais próxima da anterior.
 * n: Número de cidades.
 * m: Matriz de distâncias entre as cidades.
 * path: Vetor onde será armazenado o caminho encontrado pelo algoritmo.
 * path_cost: Variável onde será armazenado o custo do caminho path.
 */
void nn(int n, double** m, int* path, double* path_cost)
{	
	int* info;
	double min_cost;                               /* Variável para armazenar o menor custo encontrado até o momento. */
	List *whitelist, *index_list, *next_city;      /* whitelist: Ponteiro para o início da whitelist.
	                                                * index_list: Ponteiro para andar pela whitelist.
	                                                * next_city: Ponteiro para o elemento da whitelist que será incluso em path.
	                                                */
	
	whitelist = createWhitelist(n);                /* Cria uma whitelist, armazenando as cidades ainda não visitadas. */

	*path = 0;                                     /* Inicializa o caminho com a cidade 0. */
	*path_cost = 0.0;
	
	
	while(!emptyList(whitelist))                    
	{
		next_city = whitelist;
	
		info = (int *) getInfo(next_city);          
		min_cost = m[*path][*info];                /* Inicializa min_cost com o custo da cidade atual de path até uma próxima arbitrária. */
	
		index_list = next(whitelist);          
	
		while(!emptyList(index_list))              /* Anda por toda a whitelist. */
		{
			info = (int *) getInfo(index_list);
			if(m[*path][*info] < min_cost)         /* Caso o custo da cidade atual de path até uma cidade de whitelist seja menor que min_cost. */
			{
				next_city = index_list;            /* Atualiza next_city. */
				min_cost = m[*path][*info];        /* Atualiza min_cost. */
			}
		
			index_list = next(index_list);
		}
	
		*path_cost += min_cost;                    /* Atualiza path_cost. */
		info = (int *) getInfo(next_city);
		*(++path) = *info;                         /* Adiciona a cidade next_city ao path. */
	
		free(removeNode(&whitelist, next_city));   /* Remove a cidade next_city da whitelist. */
	}
	
	*path_cost += m[*path][0];                     /* Adiciona à path_cost a distância da última cidade de path até a cidade inicial 0. */
}


/*
 * Função que cria a whitelist de n - 1 cidades (Exceto a inicial, 0).
 * n: Número de cidades.
 * retorno: Retorna a whitelist.
 */
static List* createWhitelist(int n)
{
	int i;
	int* info;
	
	List* new = createEmptyList();

	for(i = n - 1; i > 0; i--)                      
	{
		info = (int *) malloc(sizeof(int));
		
		if(info == NULL)
		{	
			fprintf(stderr, "\nError while allocating memory.\n\n");
			exit(EXIT_FAILURE);
		}
		
		*info = i;                   /* A informação da lista será um ID para a cidade. */
		
		insertBegList(&new, info);
	}
	
	return new;
}


