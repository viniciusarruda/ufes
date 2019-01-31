/*
 * Aluno: Vinicius Ferraço Arruda 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com 
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 27 de novembro de 2015
 */

#ifndef SORT_H
#define SORT_H

void bubble(unsigned int* numbers, unsigned int n);
void shake(unsigned int* numbers, unsigned int n);
void insertion(unsigned int* numbers, unsigned int n);
void shell(unsigned int* numbers, unsigned int n);
void selection(unsigned int* numbers, unsigned int n);
void rank(unsigned int* numbers, unsigned int n);
void merge(unsigned int* numbers, unsigned int n);
void quickprimeiro(unsigned int* numbers, unsigned int n);
void quickcentral(unsigned int* numbers, unsigned int n);
void quickrandom(unsigned int* numbers, unsigned int n);
void quickmediana3(unsigned int* numbers, unsigned int n);
void heap(unsigned int* numbers, unsigned int n);

#endif
