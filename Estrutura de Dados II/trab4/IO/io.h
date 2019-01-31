/*
 * Aluno: Vinicius Ferraço Arruda 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com 
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 27 de novembro de 2015
 */

#ifndef IO_H
#define IO_H

#include <stdio.h>

typedef struct number
{
	unsigned int value;
	struct number* next;
} Number;

Number* readNumbersList(FILE* stream, unsigned int n);
void freeNumbersList(Number* numbers);
void printNumbersList(FILE* stream, Number* numbers);
unsigned int* readNumbers(FILE* stream, unsigned int n);
void printNumbers(FILE* stream, unsigned int* numbers, unsigned int n);

#endif
