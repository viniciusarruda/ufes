/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 12 de outubro de 2015
 */

#ifndef SEARCH_H               
#define SEARCH_H

#include "../GenericList/GenericList.h"
#include "../Word/word.h"
#include "../Btree/Btree.h"


List* getQueries(FILE* stream);
void runSearch(List* queries, Documents* documents, void* haystack, Word* (* search) (void *, char *));
void runBtreeSearch(List* queries, Documents* documents, Btree* haystack, int (* cmp) (void *, void *));

#endif
