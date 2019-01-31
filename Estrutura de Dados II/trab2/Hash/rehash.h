/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 12 de outubro de 2015
 */

#ifndef REHASH_H               
#define REHASH_H


#include "../Word/word.h"


#ifndef HASH_L_T               
#define HASH_L_T

typedef struct   /* Rehash utiliza a mesma estrutura da hash linear. */
{
	size_t size;
	Word** table;
} Hash_l;

#endif

Hash_l* createRehash(size_t size);
void insertRehash(void* h, void* w);
Word* searchRehash(void* haystack, char* string);
Hash_l* getRehash(Btree* btree, size_t size);
void freeRehash(void* h, void (* free_hash_info) (void *));
void* mountRehash(FILE* stream, Documents** documents);
void flushRehash(FILE* stream, Documents* documents, Hash_l* hash);

#endif
