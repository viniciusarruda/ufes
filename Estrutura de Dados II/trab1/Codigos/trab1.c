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

#include "exato.h"
#include "nn.h"
#include "opt.h"
#include "hull.h"


typedef enum {EXATO, NN, OPT, HULL} Algorithm_type; 
                                    
void initDistanceVariables(double*** m, int** path, int n);
void initPointVariables(Point*** p_dist, int n);   
                          
void freeDistanceVariables(double** m, int* path, int n);          
void freePointVariables(Point** p_dist, int n);

void printATSP_Path(int* path, double path_cost, int n);
void printHullPath(Point** p_dist, double path_cost, int n);


/*
 * Função main.
 */
int main(int argc, char** argv)
{
	int n;                           /* Número de cidades. */
	double path_cost;                /* Custo do caminho. */
	int* path;                       /* Caminho. (Para o caso dos algoritmos exato, nn e opt) */
	double** m;                      /* Matriz de distâncias. (Para o caso dos algoritmos exato, nn e opt) */
	Point** p_dist;                  /* Vetor de pontos. (Para o caso do algoritmo hull) */
	Algorithm_type algorithm;        
	
	
	if(argc < 3)
	{
		fprintf(stderr, "Usage: %s n algorithm\n", argv[0]);
		return 0;
	}
	
	n = atoi(argv[1]);
	
	if(n < 2)                        /* Caso houver menos de duas cidades, o programa é encerrado. */
	{
		printf("*\n");
		return 0;
	}
	
	
	if(strcmp("exato", argv[2]) == 0)
	{
		algorithm = EXATO;  
	}
	else if(strcmp("nn", argv[2]) == 0)
	{
		algorithm = NN;
	}
	else if(strcmp("opt", argv[2]) == 0)
	{
		algorithm = OPT;
	}
	else if(strcmp("hull", argv[2]) == 0)
	{
		algorithm = HULL;
	}
	else
	{
		fprintf(stderr, "Algorithm %s does not exist.\n", argv[2]);
		return 0;
	}
	
	

	switch(algorithm)                            
	{
		case EXATO:
			initDistanceVariables(&m, &path, n);
			exato(n, m, path, &path_cost);
			printATSP_Path(path, path_cost, n);
			freeDistanceVariables(m, path, n);  
			break;
		case NN:
			initDistanceVariables(&m, &path, n);
			nn(n, m, path, &path_cost);
			printATSP_Path(path, path_cost, n);
			freeDistanceVariables(m, path, n);  
			break;
		case OPT:
			initDistanceVariables(&m, &path, n);
			nn(n, m, path, &path_cost);
			opt(n, m, path, &path_cost);
			printATSP_Path(path, path_cost, n);
			freeDistanceVariables(m, path, n);  
			break;
		case HULL:
			initPointVariables(&p_dist, n);
			hull(n, p_dist, &path_cost);
			printHullPath(p_dist, path_cost, n);
			freePointVariables(p_dist, n);
			break;
	}


	return 0;
}


/*
 * Função que inicializa as variáveis relacionadas aos algoritmos que se baseiam em entradas de matriz de distâncias (exato, nn e opt).
 * m: Matriz de distâncias entre as cidades.
 * path: Vetor onde será armazenado o caminho encontrado pelo algoritmo.
 * n: Número de cidades.
 */
void initDistanceVariables(double*** m, int** path, int n)
{
	int i, j;

	*m = (double **) malloc((size_t) n * sizeof(double *));

	if(*m == NULL)
	{
		fprintf(stderr, "\nError while allocating memory for m (Distance matrix).\n\n");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < n; i++)
	{		
		(*m)[i] = (double *) malloc((size_t) n * sizeof(double));
	
		if((*m)[i] == NULL)
		{
			fprintf(stderr, "\nError while allocating memory for m[%d] (Distance matrix).\n\n", i);
			exit(EXIT_FAILURE);
		}
	}


	for(i = 0; i < n; i++)                                        /* Preenche a matriz recem alocada. */
		for(j = 0; j < n; j++)
			scanf("%lf", &(*m)[i][j]);
		

	*path = (int *) malloc((size_t) n * sizeof(int));

	if(*path == NULL)
	{
		fprintf(stderr, "\nError while allocating memory for path.\n\n");
		exit(EXIT_FAILURE);
	}
}	


/*
 * Função que libera as variáveis relacionadas aos algoritmos que se baseiam em entradas de matriz de distâncias (exato, nn e opt).
 * m: Matriz de distâncias entre as cidades.
 * path: Vetor do caminho encontrado pelo algoritmo.
 * n: Número de cidades.
 */
void freeDistanceVariables(double** m, int* path, int n)
{
	int i;
	
	free(path);

	for(i = 0; i < n; i++)
		free(m[i]);
		
	free(m);
}


/*
 * Função que inicializa as variáveis relacionadas aos algoritmos que se baseiam em entradas de pontos cartesianos (hull).
 * p_dist: Vetor de pontos onde será armazenado o caminho encontrado pelo algoritmo.
 * n: Número de cidades.
 */
void initPointVariables(Point*** p_dist, int n)
{
	int i;
	double x, y;

	*p_dist = (Point **) malloc((size_t) n * sizeof(Point *));
	
	if(*p_dist == NULL)
	{
		fprintf(stderr, "\nError while allocating memory for p_dist.\n\n");
		exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < n; i++)
	{
		scanf("%lf", &x);
		scanf("%lf", &y);
		
		(*p_dist)[i] = createPoint(i, OUT_PATH, x, y);       /* Cria um ponto e atribui ao vetor recem criado. */
	}
}


/*
 * Função que libera as variáveis relacionadas aos algoritmos que se baseiam em entradas de pontos cartesianos (hull).
 * p_dist: Vetor de pontos do caminho encontrado pelo algoritmo.
 * n: Número de cidades.
 */
void freePointVariables(Point** p_dist, int n)
{
	int i;
	
	for(i = 0; i < n; i++)
		free(p_dist[i]);
		
	free(p_dist);
}


/*
 * Função que imprime o caminho encontrado pelos algoritmos exato, nn e opt.
 * path: Vetor do caminho encontrado pelo algoritmo.
 * path_cost: Custo do caminho path.
 * n: Número de cidades.
 */
void printATSP_Path(int* path, double path_cost, int n)
{
	int i;
	
	for(i = 0; i < n; i++)
		printf("%d\n", path[i]);
		
	printf("%.2f\n*\n", path_cost);
}


/*
 * Função que imprime o caminho encontrado pelo algoritmo hull.
 * p_dist: Vetor de pontos do caminho encontrado pelo algoritmo.
 * path_cost: Custo do caminho p_dist.
 * n: Número de cidades.
 * Obs: A impressão do caminho sempre começa pela cidade 0.
 */
void printHullPath(Point** p_dist, double path_cost, int n)
{
	int i, init;                                /* init: Índice da cidade 0 no vetor de pontos p_dist. */
	
	i = 0;
	while(p_dist[i]->id != 0) i++;              /* Procura a cidade 0 em p_dist. */
	init = i;
	
	for(; i < n; i++)                           /* Imprime a partir da cidade 0, até o fim do vetor p_dist. */
	{
		printf("%d\n", p_dist[i]->id);
	}
	
	for(i = 0; i < init; i++)                   /* Imprime a partir do início de p_dist até um antes da cidade 0. */
	{
		printf("%d\n", p_dist[i]->id);
	}
		
	printf("%.2f\n*\n", path_cost);             /* Imprime custo deste caminho e *. */
}


