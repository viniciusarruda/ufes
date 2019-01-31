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
#include <string.h>


#define index(i) ((i) % (n))


static double cost(int n, double** m, int* path);
static void kind_of_swap(int* dest, int* src, int n, int start, int end);


/*
 * Função que otimiza um tour de n cidades. Foi implementado o algoritmo 2-opt.
 * n: Número de cidades.
 * m: Matriz de distâncias entre as cidades.
 * original_path: Vetor onde será armazenado o caminho otimizado encontrado pelo algoritmo.
 * current_cost: Variável onde será armazenado o custo do caminho original_path.
 */
void opt(int n, double** m, int* original_path, double* current_cost)
{
	int i, j, changed;   
	int* best_path;               /* Armazenará o melhor caminho encontrado até o momento. */
	int* aux_path;                /* Armazenará o caminho feito por uma troca. */
	double new_cost;              /* Armazenará o custo do caminho aux_path. */
	
	
	best_path = (int *) malloc((size_t)n * sizeof(int));
	
	if(best_path == NULL)
	{
		fprintf(stderr, "\nError while allocating memory for best path.\n\n");
		exit(EXIT_FAILURE);
	}
	
	aux_path = (int *) malloc((size_t)n * sizeof(int));
	
	if(aux_path == NULL)
	{
		fprintf(stderr, "\nError while allocating memory for aux path.\n\n");
		exit(EXIT_FAILURE);
	}
	
	
	*current_cost = cost(n, m, original_path);                      /* Calcula o custo do caminho atual. */
	memcpy(best_path, original_path, (size_t) n * sizeof(int));     /* Inicializa best_path com o caminho atual. */

	
	do
	{
		changed = 0;

		for(i = 0; i < n; i++)                                      /* Para todas as cidades. */
		{       
			for(j = i + 2; j <= n - 2 + i; j++)                     /* Para todos os pares de aresta não adjacentes. */
			{				
				kind_of_swap(aux_path, original_path, n, index(i + 1), index(j));  /* Realiza a troca. */
				
				new_cost = cost(n, m, aux_path);                    /* Calcula o custo do caminho após a troca. */
								
				if(new_cost < *current_cost)                        /* Caso o novo custo for menor que o custo passado. */
				{
					*current_cost = new_cost;                       /* Atualiza custo. */
					memcpy(best_path, aux_path, (size_t) n * sizeof(int));  /* Atualiza melhor caminho. */
					changed = 1;                                   
				}
			}
		}
		
		memcpy(original_path, best_path, (size_t) n * sizeof(int));   /* Salva o melhor caminho. Agora o caminho atual é o melhor encontrado. */  
	}
	while(changed);                                                   /* Enquanto houver mudança no caminho. */
	
	
	free(best_path);
	free(aux_path);
}


/*
 * Função que calcula o custo de um caminho path.
 * n: Número de cidades.
 * m: Matriz de distâncias entre as cidades.
 * path: Vetor que contém o caminho para avaliar seu custo.
 */
static double cost(int n, double** m, int* path)
{
	int i;
	double cost;
	
	cost = 0;  
	
	for(i = 0; i < n - 1; i++)
	{
		cost += m[path[i]][path[i + 1]];
	}
	cost += m[path[i]][path[0]];
	
	return cost;
}


/*
 * Função que realiza a inversão de um trecho do caminho src e o coloca em dest.
 * dest: Vetor de destino do caminho com trecho invertido.
 * src: Vetor de origem do caminho a ser invertido.
 * n: Número de cidades.
 * start: Índice de onde começa a inversão do caminho src.
 * end: Índice de onde termina a inversão do caminho src.
 */
static void kind_of_swap(int* dest, int* src, int n, int start, int end)
{
	int i;
	
	for(i = 0; i < start; i++)    /* Copia o intervalo 0 até start do caminho src para dest. */
	{
		dest[i] = src[i];
	}
	
	while(start <= end)           /* Inverte o intervalo start até end do caminho src e o coloca em dest. */
	{
		dest[i] = src[end];
		i++;
		end--;
	}
	
	for(; i < n; i++)             /* Copia o intervalo end até n do caminho src para dest. */
	{
		dest[i] = src[i];
	}
}


