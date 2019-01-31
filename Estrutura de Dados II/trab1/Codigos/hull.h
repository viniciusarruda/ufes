/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professora: Mariella Berger 
 * Data: 02 de Setembro de 2015
 */


#ifndef HULL_H                                    
#define HULL_H                                    


#define IN_PATH  1
#define OUT_PATH 0

                           
typedef struct 
{
	int id;           /* Id do ponto. */
	int state;        /* Estado do ponto. (No caminho ou fora do caminho) */
	double x;         /* Posição x do ponto. */
	double y;         /* Posição y do ponto. */
} Point;


Point* createPoint(int id, int state, double x, double y);
int compar(const void* p1 , const void* p2);
double determinante(Point* p1, Point* p2, Point* p3);

void hull(int n, Point** p_dist, double* path_cost);


#endif 

