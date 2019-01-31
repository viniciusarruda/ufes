/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 12 de outubro de 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "hashEncadeada.h"
#include "hashUtil.h"
#include "../Btree/Btree.h"
#include "../Word/word.h"


static List* mountList(FILE* stream);
static void flushList(FILE* stream, List* l);


Hash_e* createHashEncadeada(size_t size)
{
	size_t i;
	Hash_e* new;
	
	
	if(size > ((size_t) -1) / 2)
	{
		fprintf(stderr, "Size for hash not supported.");
		exit(EXIT_FAILURE);
	}

	size = ceil_prime((size_t) ((double) size * 10.0 / 7.0)); /* 70 % de palavras */
	
	new = (Hash_e *) malloc(sizeof(Hash_e));
	
	if(new == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}
	
	new->table = (List **) malloc(size * sizeof(List *));
	
	if(new->table == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < size; i++)
		new->table[i] = createEmptyList(); 
	
	new->size = size;

	return new;
}


void freeHashEncadeada(void* h, void (* free_hash_info) (void *))
{
	size_t i;
	Hash_e* hash = h;

	for(i = 0; i < hash->size; i++)
	{
		freeList(&hash->table[i], free_hash_info);
	}

	free(hash->table);
	free(hash);
}


Hash_e* getHashEncadeada(Btree* btree, size_t size)
{
	Hash_e* hash;
	
	hash = createHashEncadeada(size);

	convertBtree(btree, hash, insertHashEncadeada);

	return hash;
}


void insertHashEncadeada(void* h, void* w)
{
	Word* word = w;
	Hash_e* hash = h;

	insertBegList(&hash->table[hash_function(word->word) % hash->size], word);
}


static int equals(void* info, void* string)
{
	Word* w = info;

	return !strcmp(w->word, (char *) string);
}


Word* searchHashEncadeada(void* haystack, char* string)
{
	Hash_e* hash = (Hash_e *) haystack;

	return (Word *) searchInfo(hash->table[hash_function(string) % hash->size], string, equals);
}


static List* mountList(FILE* stream)
{
	Word* w;
	List* new = createEmptyList();

	while((w = mountWord(stream)) != NULL)    
	{
		insertBegList(&new, w);
	}

	return new;
}


static void flushList(FILE* stream, List* l)
{
	Word* w;

	while((w = removeBegList(&l)) != NULL)
	{
		flushWord(stream, w);
	}
	fprintf(stream, "\n");
}


void flushHashEncadeada(FILE* stream, Documents* documents, Hash_e* hash)
{
	size_t i;

	flushDocuments(stream, documents);

	fprintf(stream, "%lu\n", hash->size);

	for(i = 0; i < hash->size; i++)
	{
		flushList(stream, hash->table[i]);
	}

	free(hash->table);
	free(hash);
}


void* mountHashEncadeada(FILE* stream, Documents** documents)
{
	size_t i;
	Hash_e* hash;

	mountDocuments(stream, documents);

	hash = (Hash_e *) malloc(sizeof(Hash_e));
	
	if(hash == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}
	
	fscanf(stream, "%lu", &hash->size);
	fgetc(stream); /* skips \n */

	hash->table = (List **) malloc(hash->size * sizeof(List *));
	
	if(hash->table == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}


	for(i = 0; i < hash->size; i++)
	{
		hash->table[i] = mountList(stream);
	}

	return hash;
}

