/*
 * Aluno: Vinicius Ferraço Arruda e Guilherme Sfalsin Scopel 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com e scopel.18@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 10 de outubro de 2015
 */

#ifndef BS_H
#define BS_H

#include "../Job/job.h"
#include "../Solution/solution.h"

void beamSearchDepth(Job** jobs, unsigned int n, Solution* solution, unsigned int beam_width);
void beamSearchBreadth(Job** jobs, unsigned int n, Solution* solution, unsigned int beam_width);

#endif

