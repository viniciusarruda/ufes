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


static void exato_rec(int k, int n, int* path, int* min_path, double** m, double sum, double* min_cost);


/*
 * Função que, junto à exato_rec(), calcula o menor caminho para o problema do caixeiro viajante.
 * n: Número de cidades.
 * m: Matriz de distâncias entre as cidades.
 * min_path: Vetor onde será armazenado o menor caminho encontrado pelo algoritmo.
 * min_cost: Variável onde será armazenado o custo do caminho min_path.
 */
void exato(int n, double** m, int* min_path, double* min_cost)
{
	int i;                                               /* Variável utilizada para índice no loop for. */
	int* path;                                           
	
	path = (int *) malloc((size_t) n * sizeof(int));     /* Vetor auxiliar para construir o caminho. */

	if(path == NULL)
	{
		fprintf(stderr, "\nError while allocating memory for path.\n\n");
		exit(EXIT_FAILURE);
	}
	

	path[0] = 0;                                         /* Inicializa o caminho com 0, pois este algoritmo parte sempre da cidade 0. */
	*min_cost = 0.0;                                     /* Inicializa custo mínimo do caminho com 0.0. */
	
	for(i = 1; i < n; i++)                               /* Preenche path com o caminho inicial sendo 0, 1, 2.. n. */
	{			
		path[i] = i;
		*min_cost += m[i - 1][i];                        /* Calcula o custo do caminho contido no vetor path. */
	}
	*min_cost += m[i - 1][0];

	
	memcpy(min_path, path, (size_t) n * sizeof(int));    /* Inicializa min_path com o caminho contido em path. */
	
	
	exato_rec(n - 1, n, &path[1], min_path, m, 0, min_cost);   /* Chama exato_rec(). */
	
	
	free(path);                                          /* Libera a memória ocupada pelo vetor auxiliar path. */
}


/*
 * Função que gera recursivamente todas as possibilidades de caminho e calcula seu custo.
 * k: Contador que armazena o número de cidades restantes para analisar as possibilidades.
 * n: Número de cidades.
 * path: Vetor auxiliar para gerar as possibilidades de caminho.
 * min_path: Vetor onde será armazenado o menor caminho encontrado pelo algoritmo.
 * m: Matriz de distâncias entre as cidades.
 * sum: Acumula a soma das distâncias do caminho contido em path.
 * min_cost: Variável onde será armazenado o custo do caminho min_path.
 *
 * WARNING: Esta função é chamada de maneira recursiva. Logo, a pilha de execução terá 1 chamada à exato() seguido de n - 1 chamadas empilhadas 
 *          de exato_rec(), totalizando n funções empilhadas. Estas funções, em especial exato_rec(), possuem grande quantidade de parêmetros, 
 *          e exijirá uma quantia maior de memória da pilha.
 *          Caso o número de cidades seja muito grande, poderá ocorrer Stack Overflow.
 */
static void exato_rec(int k, int n, int* path, int* min_path, double** m, double sum, double* min_cost) 
{
	int i, tmp;        
	
	if(k == 1)                                    /* Caso for a vez de analisar a última cidade. (Restou apenas uma cidade para analisar) */
	{
		sum += m[*(path - 1)][*path] + m[*path][0];   /* Acumula o custo do caminho da última cidade em path para a cidade 0. */
		
		if(sum < *min_cost)                       /* Caso o custo do caminho contido em path seja menor do que o custo contido em min_path. */
		{
			*min_cost = sum;                      /* Atualiza min_path para path e min_cost para sum. */
			memcpy(min_path, path - n + 1, (size_t) n * sizeof(int));
		}
	}
	else
	{
		exato_rec(k - 1, n, path + 1, min_path, m, m[*(path - 1)][*path] + sum, min_cost);

		for(i = 1; i < k; i++)
		{
			tmp = path[i];                        /* Faz o swap da posição corrente de path com a posição path[i]. */
			path[i] = *path;	
			*path = tmp;

			exato_rec(k - 1, n, path + 1, min_path, m, m[*(path - 1)][*path] + sum, min_cost); 
			
			tmp = path[i];                        /* Desfaz o swap. */
			path[i] = *path;		
			*path = tmp;
		}
	}
}


