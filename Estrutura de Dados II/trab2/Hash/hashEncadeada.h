/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 12 de outubro de 2015
 */

#ifndef HASHENCADEADA_H               
#define HASHENCADEADA_H

#include "../Btree/Btree.h"
#include "../Word/word.h"


typedef struct
{
	size_t size;
	List** table;
} Hash_e;


void freeHashEncadeada(void* h, void (* free_hash_info) (void *));
Hash_e* getHashEncadeada(Btree* btree, size_t size);
void insertHashEncadeada(void* h, void* w);
Word* searchHashEncadeada(void* haystack, char* string);
void flushHashEncadeada(FILE* stream, Documents* documents, Hash_e* hash);
void* mountHashEncadeada(FILE* stream, Documents** documents);


#endif
