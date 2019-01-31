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
#include <math.h>

#include "hull.h"
#include "Stack/Stack.h"
#include "GenericList/GenericList.h"


static Stack* createConvexHull(Point** p_dist, int n);
static void completeTour(List* path, List** out_path);
static double custoDesvio(Point* p1, Point* out, Point* p2);
static void calcCost(Point** p_dist, double* path_cost, int n);


/*
 * Função que, junto à createConvexHull() e completeTour(), geram um caminho para o problema do caixeiro viajante.
 * n: Número de cidades.
 * p_dist: Vetor de pontos (cidades) no plano cartesiano.
 * path_cost: Armazenará o custo do caminho gerado.
 * retorno: Não retorna nada. Porém, reorganiza o vetor p_dist na ordem do caminho gerado.
 */
void hull(int n, Point** p_dist, double* path_cost)
{
	int i;
	Stack* s;
	List* path;                                                  /* Lista de cidades que estão no caminho. */
	List* out_path;	                                             /* Lista de cidades que não estão no caminho. */
	List* saveptr;
	
	qsort(p_dist, (size_t) n, sizeof(Point *), compar);          /* Ordena o vetor p_dist de acordo com compar. */

	s = createConvexHull(p_dist, n);                             /* Cria um envoltório convexo e o armazena em uma pilha. */
		
	path = createEmptyList();                                     
	
	while(!emptyStack(s))
	{
		insertBegList(&path, pop(&s));                           /* Insere cada ponto da pilha na lista path. */
	}
	
	out_path = createEmptyList();
	
	for(i = 0; i < n; i++)
	{
		if(p_dist[i]->state == OUT_PATH)                         /* Insere cada ponto que não está no caminho na lista out_path. */
		{
			insertBegList(&out_path, p_dist[i]);
		}
	}

	completeTour(path, &out_path);                               /* Completa o tour, colocando todos os pontos de out_path em path. */

	i = 0;
	saveptr = path;
	while(!emptyList(saveptr))                                   /* Insere os pontos de path em p_dist na ordem do tour. */
	{
		p_dist[i] = (Point *) getInfo(saveptr);
		i++;
		saveptr = next(saveptr);
	}
	
	calcCost(p_dist, path_cost, n);                              /* Calcula o custo do caminho encontrado. */

	freeStack(&s);
	freeList(&path);
	freeList(&out_path);
}


/*
 * Função que gera o envoltório convexo.
 * p_dist: Vetor de pontos (cidades) no plano cartesiano ordenados em ordem crescente em relação ao eixo x.
 * n: Número de cidades.
 * retorno: Retorna uma pilha, com o caminho do envoltório empilhado ponto a ponto.
 */	
static Stack* createConvexHull(Point** p_dist, int n)
{
	int i;
	Stack* s;
	Point *p1, *p2, *p3, *tmp, *end;             /* p1, p2, e p3 são ponteiros para pontos para auxiliar na construção do envoltório. */
	                                             /* end armazena o último ponto a entrar na parte superior do envoltório convexo. */
	
	s = createEmptyStack();

	p1 = p_dist[0];                              /* Coloca o primeiro e o segundo ponto na pilha. */
	p1->state = IN_PATH;
	push(&s, p1);
	
	p2 = p_dist[1];
	p2->state = IN_PATH;
	push(&s, p2);
	
	
	for(i = 2; i < n; i++)                       /* Faz a parte superior do envoltório convexo. */
	{
		p3 = p_dist[i];                          

		while(determinante(p1, p2, p3) > 0.0)    /* Enquanto os pontos p1, p2 e p3 formarem uma curva côncava. */
		{                                   
			tmp = (Point *) pop(&s);             /* Retira da pilha o ponto que estava gerando a curva côncava. */
			tmp->state = OUT_PATH;
			p2 = (Point *) lookStackInfo(s, 0);  /* Atualiza p2 e p1. */
			p1 = (Point *) lookStackInfo(s, 1);
			
			if(p1 == NULL)                       /* Caso não houver mais pontos a serem retirados da pilha. */
				break;
		}
		
		p3->state = IN_PATH;                     /* Coloca p3 na pilha. */
		push(&s, p3);
		
		p1 = p2;                                 /* Atualiza p1 e p2. */
		p2 = p3;
	}
	
	
		
	p1 = p3;                                     /* Faz a parte inferior do envoltório convexo. */
	end = p3;                                    /* Atualiza p1. Marca o último ponto da parte superior do envoltório. */
	
	i = n - 2;

	while(p_dist[i]->state == IN_PATH && i > 0) i--;  /* Procura, de trás para frente, um ponto fora do caminho em p_dist. */

	if(i != 0)                                        /* Se foi encontrado um ponto. */
	{
		p2 = p_dist[i];                               /* Coloca ele na pilha. */
		p2->state = IN_PATH;
		push(&s, p2);
	
		for(; i >= 0; i--)                            
		{
			while(p_dist[i]->state == IN_PATH && i > 0) i--;  /* Procura, de trás para frente, um ponto fora do caminho em p_dist. */
			
			p3 = p_dist[i];
		
			while(determinante(p3, p2, p1) < 0.0)             /* Enquanto os pontos p1, p2 e p3 formarem uma curva côncava. */
			{                                   
				tmp = (Point *) pop(&s);                      /* Retira da pilha o ponto que estava gerando a curva côncava. */
				tmp->state = OUT_PATH;
				p2 = (Point *) lookStackInfo(s, 0);           /* Atualiza p2. */
			
				if(p2->x == end->x && p2->y == end->y)        /* Caso não houver mais pontos a serem retirados da pilha. */
					break;
			
				p1 = (Point *) lookStackInfo(s, 1);           /* Atualiza p1. */
			}
		
			if(i != 0)                                        /* Caso p3 não seja o primeiro ponto da parte superior. */
			{
				p3->state = IN_PATH;                          /* Coloca ele na pilha. */
				push(&s, p3);
		
				p1 = p2;                                      /* Atualiza p1 e p2. */
				p2 = p3;
			}
		}
	}
	
	return s;                                                 /* Retorna a pilha com o envoltório convexo. */
}


/*
 * Função que gera um caminho a partir do envoltório convexo.
 * path: Lista de pontos que estão no caminho.
 * out_path: Lista de pontos que não estão no caminho.
 */	
static void completeTour(List* path, List** out_path)
{
	double min, tmp;                              /* min: Armazena o menor custo de desvio já encontrado. */
	Point *p1, *p2, *p_tmp;                       /* p1 e p2 armazenam pontos adjacentes da lista de pontos que estão no caminho. */
	List *outTour;                                /* Mantém referência para o nó que vai sair de out_path. */
	List* ref_node;                               /* Mantém referência para um nó, que após ele, será inserido um novo ponto. */
	List *out_index, *in_index;                   /* out_index: Índice da lista de pontos que não estão do caminho. */
	                                              /* in_index: Índice da lista de pontos que estão do caminho. */


	while(!emptyList(*out_path))                  /* Enquanto houver algum ponto em out_path. */
	{	
		out_index = *out_path;                    /* Inicializa out_index. */
	
		while(!emptyList(out_index))              /* Enquanto não chegar ao fim da lista de pontos que não estão no caminho. */
		{
			in_index = path;                      /* Inicializa in_index. */
		
			p1 = getInfo(in_index);               /* p1 recebe o primeiro ponto de in_index. */
			ref_node = in_index;                  
			in_index = next(in_index);            
			p2 = getInfo(in_index);               /* p2 recebe o segundo ponto de in_index. */
			
			min = custoDesvio(p1, getInfo(out_index), p2);  /* Calcula custo do desvio. */
		
			outTour = out_index;                  /* Marca o ponto que vai sair. */

			
			while(!emptyList(next(in_index)))     /* Enquanto não chegar ao fim da lista de pontos que estão no caminho. */
			{
				p1 = p2;                          /* Atualiza p1 e p2. */
				p2 = getInfo(next(in_index)); 
				
				tmp = custoDesvio(p1, getInfo(out_index), p2);   /* Calcula custo do desvio. */
			
				if(tmp < min)                     /* Caso tmp for menor que o menor custo de desvio já encontrado. */ 
				{
					min = tmp;                    /* Atualiza menor custo de desvio. */
			
					ref_node = in_index;          /* Atualiza ref_node e outTour. */
					outTour = out_index;
				}
				
				in_index = next(in_index);
			}

			                                      /* Ao acabar a lista in_index, checa com o último ponto e o primeiro. */
			p1 = p2;
			p2 = getInfo(path);                   /* path é o primeiro elemento da lista. */

			tmp = custoDesvio(p1, getInfo(out_index), p2);   /* Calcula custo do desvio. */
			
			if(tmp < min)                         /* Caso tmp for menor que o menor custo de desvio já encontrado. */ 
			{
				min = tmp;                        /* Atualiza menor custo de desvio. */
				
				ref_node = in_index;              /* Atualiza ref_node e outTour. */
				outTour = out_index;
			}
			

			out_index = next(out_index); 
		}
		
		p_tmp = (Point *) removeNode(out_path, outTour);   /* Remove outTour da lista out_path. */
		p_tmp->state = IN_PATH;
		insertAfterNode(ref_node, p_tmp);                  /* Insere o ponto p_tmp no caminho. */
	}
}


/*
 * Função que calcula o custo do desvio de caminho entre p1 -> p2 para p1 -> out -> p2.
 * p1: Ponto origem.
 * p2: Ponto destino.
 * out: Ponto de desvio.
 * retorno: Retorna o custo.
 */	
static double custoDesvio(Point* p1, Point* out, Point* p2)  
{
	double tmp, x, y;
	
	x = fabs(p1->x - out->x);
	y = fabs(p1->y - out->y);
	
	tmp = sqrt(x*x + y*y);     
	
	x = fabs(p2->x - out->x);
	y = fabs(p2->y - out->y);
	
	tmp += sqrt(x*x + y*y);
	
	x = fabs(p1->x - p2->x);
	y = fabs(p1->y - p2->y);
	
	return tmp - sqrt(x*x + y*y); 
}


/*
 * Função que calcula o custo total do caminho.
 * p_dist: Armazena o caminho em uma lista de pontos.
 * path_cost: Armazenará o custo total do caminho.
 * n: Número de cidades.
 */	
static void calcCost(Point** p_dist, double* path_cost, int n)
{
	int i;
	double x, y;

	*path_cost = 0;

	for(i = 0; i < n - 1; i++)
	{
		x = fabs(p_dist[i]->x - p_dist[i + 1]->x);     /* Calcula a distância euclidiana entre os pontos. */
		y = fabs(p_dist[i]->y - p_dist[i + 1]->y);
		
		*path_cost += sqrt(x*x + y*y);
	}
	
	x = fabs(p_dist[i]->x - p_dist[0]->x);
	y = fabs(p_dist[i]->y - p_dist[0]->y);
	
	*path_cost += sqrt(x*x + y*y);
}


/*
 * Função que compara se um ponto é maior que outro em relação à x.
 * p1: Ponto 1.
 * p2: Ponto 2.
 * retorno: 
 *       -1 se p1(x) for menor que p2(x).
 *        1 se p1(x) for maior que p2(x). 
 *        0 se p1(x) e p2(x) forem iguais. 
 */	
int compar(const void* p1 , const void* p2)
{
	const Point** point1 = (const Point**) p1;
	const Point** point2 = (const Point**) p2;

	if((*point1)->x < (*point2)->x)
		return -1;
	else if ((*point1)->x > (*point2)->x)
		return 1;
	else 
		return 0;
}


/*
 * Função que calcula o determinante entre 3 pontos.
 * p1, p2 e p3 são os pontos.
 * retorno: Retorna o valor do determinante.
 */	
double determinante(Point* p1, Point* p2, Point* p3)
{
	return (p2->x - p1->x) * (p3->y - p1->y) - (p2->y - p1->y) * (p3->x - p1->x);
}


/*
 * Função que cria um ponto.
 * id: Identificador do ponto.
 * state: Estado atual do ponto.
 * x: valor na coordenada x do ponto.
 * y: valor na coordenada y do ponto.
 * retorno: Retorna um ponteiro para o ponto recem criado.
 */	
Point* createPoint(int id, int state, double x, double y)
{
	Point* new = (Point *) malloc(sizeof(Point));
	
	if(new == NULL)
	{
		fprintf(stderr, "\nError while allocating memory.\n\n");
		exit(EXIT_FAILURE);
	}

	new->id = id;
	new->state = state;
	new->x = x;
	new->y = y;
	
	return new;
}


