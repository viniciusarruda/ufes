/*
 * Aluno: Vinicius Ferraço Arruda e Guilherme Sfalsin Scopel 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com e scopel.18@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 10 de outubro de 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "Job/job.h"
#include "BeamSearch/beamSearch.h"
#include "BranchBound/branchBound.h"
#include "Solution/solution.h"


/*
 * main: Confere os argumentos de entrada e se corretos, chama a função com os devidos parâmetros.
 */
int main(int argc, char** argv)
{
	unsigned int n;            /* Número de Jobs. */
	Job** jobs;          /* Array de ponteiros para Job. */
	Solution* solution;  /* Ordem que os jobs serão executados. */
	                     /* Custo do melhor sequenciamento de jobs. (Custo da ordem do schedule) */

	if(argc != 3)  
	{
		fprintf(stdout, "Usage: %s [Numero de jobs] [Algoritmo]\n", argv[0]);
		return 0;
	}

	if(sscanf(argv[1], "%u", &n) != 1 || n == 0)
	{
		fprintf(stdout, "O numero de Jobs deve ser um valor inteiro maior que 0.\n");
		return 0;	
	}

	if(n >= UINT_MAX)  /* Tratando um possivel erro acima. Caso valor digitado for negativo, vai converter para um número muito grande. */
	{
		fprintf(stdout, "Numero de Jobs nao suportado.\n");
		return 0;	
	}


	if(strlen(argv[2]) == 2 && argv[2][0] == 'b')
	{
		if(argv[2][1] == 's')               /* Algoritmo: bs (Beam Search) */
		{
			solution = newSolution(n);
			jobs = getJobs(stdin, n);
			beamSearchBreadth(jobs, n, solution, 25);
			printSolution(solution, n);
			freeSolution(solution);
			freeJobs(jobs, n);
		}
		else if(argv[2][1] == 'b')          /* Algoritmo: bb (Branch and Bound) */
		{
			solution = newSolution(n);
			jobs = getJobs(stdin, n);
			beamSearchBreadth(jobs, n, solution, 25);
			branchBound(jobs, n, solution);
			printSolution(solution, n);
			freeSolution(solution);
			freeJobs(jobs, n);
		}
		else
		{
			fprintf(stdout, "Algoritmo [%s] invalido.\n", argv[2]);
			return 0;	
		}
	}
	else
	{
		fprintf(stdout, "Algoritmo [%s] invalido.\n", argv[2]);
		return 0;
	}

	return 0;
}

