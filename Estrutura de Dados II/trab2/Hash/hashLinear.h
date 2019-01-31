/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 12 de outubro de 2015
 */

#ifndef HASHLINEAR_H               
#define HASHLINEAR_H


#include "../Word/word.h"


#ifndef HASH_L_T               
#define HASH_L_T

typedef struct
{
	size_t size;
	Word** table;
} Hash_l;

#endif


Hash_l* createHashLinear(size_t size);
void insert_hash(void* h, void* w);
Word* searchHashLinear(void* haystack, char* string);
Hash_l* getHashLinear(Btree* btree, size_t size);
void freeHashLinear(void* h, void (* free_hash_info) (void *));
void* mountHashLinear(FILE* stream, Documents** documents);
void flushHashLinear(FILE* stream, Documents* documents, Hash_l* hash);

#endif
