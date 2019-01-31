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


#include "hashUtil.h"
#include "hashLinear.h"
#include "../Word/word.h"



Hash_l* createHashLinear(size_t size)
{
	size_t i;
	Hash_l* new;
	
	
	if(size > ((size_t) -1) / 2)
	{
		fprintf(stderr, "Size for hash not supported.");
		exit(EXIT_FAILURE);
	}

	size = ceil_prime((size_t) ((double) size * 10.0 / 7.0)); /* 70 % de palavras (load factor) */
	
	new = (Hash_l *) malloc(sizeof(Hash_l));
	
	if(new == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}
	
	new->table = (Word **) malloc(size * sizeof(Word *));
	
	if(new->table == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < size; i++)
		new->table[i] = NULL; /* not safe to memset(zeros).. null is not aways 0 */
	
	new->size = size;

	return new;
}


Hash_l* getHashLinear(Btree* btree, size_t size)
{
	Hash_l* hash;

	hash = createHashLinear(size);

	convertBtree(btree, hash, insert_hash);

	return hash;
}


void freeHashLinear(void* h, void (* free_hash_info) (void *))
{
	size_t i;
	Hash_l* hash = h;

	for(i = 0; i < hash->size; i++)
	{
		if(hash->table[i] != NULL)
			free_hash_info(hash->table[i]);
	}

	free(hash->table);
	free(hash);
}
	
	
void insert_hash(void* h, void* w)
{
	Hash_l* hash = h;
	Word* word = w;
	size_t index, saveindex;

	index = hash_function(word->word) % hash->size;
	
	if(hash->table[index] == NULL)
	{
		hash->table[index] = word;
		return;
	}
	
	saveindex = index;
	do
		index = (index + 1) % hash->size; 
	while(hash->table[index] != NULL && index != saveindex);
	
	
	if(hash->table[index] != NULL)
	{
		fprintf(stderr, "Hash FULL !\n");  /* Este caso nunca irá ocorrer, pois o load factor é 70% */
		exit(EXIT_FAILURE);
	}
	
	hash->table[index] = word;
}


Word* searchHashLinear(void* haystack, char* string)
{
	Hash_l* hash = (Hash_l *) haystack;
	size_t index, saveindex;

	index = hash_function(string) % hash->size;


	if(hash->table[index] != NULL && !strcmp(hash->table[index]->word, string))
	{
		return hash->table[index];
	}

	
	saveindex = index;
	do
		index = (index + 1) % hash->size;  
	while(hash->table[index] != NULL && index != saveindex && strcmp(hash->table[index]->word, string));
	
	
	
	if(hash->table[index] == NULL || index == saveindex) 
	{
		return NULL;
	}
	
	return hash->table[index];
}


void* mountHashLinear(FILE* stream, Documents** documents)
{
	int c;
	size_t i;
	char* str;
	Hash_l* hash;
	Word* word;
	Doc* doc;
	size_t docID;
	size_t index;


	*documents = (Documents *) malloc(sizeof(Documents));

	if(*documents == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	fscanf(stream, "%lu", &(*documents)->size);
	fgetc(stream); /* skips \n */

	(*documents)->documents = (char **) malloc((*documents)->size * sizeof(char *));

	if((*documents)->documents == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < (*documents)->size; i++)
	{
		(*documents)->documents[i] = getString(stream);
	}
	

	hash = (Hash_l *) malloc(sizeof(Hash_l));
	
	if(hash == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}
	
	fscanf(stream, "%lu", &hash->size);
	fgetc(stream); /* skips \n */

	hash->table = (Word **) malloc(hash->size * sizeof(Word *));
	
	if(hash->table == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}


	for(i = 0; i < hash->size; i++)
	{
		if((str = getString(stream)) == NULL)
		{
			hash->table[i] = NULL;
		}
		else
		{
			word = (Word *) malloc(sizeof(Word));

			if(word == NULL)
			{
				fprintf(stderr, "Out of memory.\n");
				exit(EXIT_FAILURE);
			}

			word->word = str;
			word->doc = createEmptyList();


			while((c = fgetc(stream)) != '\n') 
			{
				ungetc(c, stream);

				fscanf(stream, "%lu", &docID);
				fscanf(stream, "%lu", &index);  

				doc = create_doc(docID, create_index(index));

				while((c = fgetc(stream)) != '\n')
				{
					ungetc(c, stream);

					fscanf(stream, "%lu", &index); 

					insertBegList(&doc->index, create_index(index));
				}
				
				insertBegList(&word->doc, doc);
			}

			hash->table[i] = word;	
		}
	} 
	
	return hash;
}


void flushHashLinear(FILE* stream, Documents* documents, Hash_l* hash)
{
	size_t i;
	Doc* doc;
	size_t* index;


	fprintf(stream, "%lu\n", documents->size);
	for(i = 0; i < documents->size; i++)
	{
		fprintf(stream, "%s\n", documents->documents[i]);
		free(documents->documents[i]);	
	}
	free(documents->documents);
	free(documents);


	fprintf(stream, "%lu\n", hash->size);

	for(i = 0; i < hash->size; i++)
	{
		if(hash->table[i] != NULL)
		{
			fprintf(stream, "%s\n", hash->table[i]->word);

			while(!emptyList(hash->table[i]->doc))
			{
				doc = (Doc *) removeBegList(&hash->table[i]->doc);

				fprintf(stream, " %lu", doc->docID);

				while(!emptyList(doc->index))
				{
					index = (size_t *) removeBegList(&doc->index);

					fprintf(stream, " %lu", *index);

					free(index);
				}
				fprintf(stream, "\n");
				free(doc);
			}
			
			free(hash->table[i]->word);
			free(hash->table[i]);
		}
		fprintf(stream, "\n");
	}

	free(hash->table);
	free(hash);
}
	
	
	