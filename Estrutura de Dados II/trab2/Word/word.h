/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 12 de outubro de 2015
 */

#ifndef WORD_H               
#define WORD_H

#include <wchar.h>
#include "../GenericList/GenericList.h"
#include "../Btree/Btree.h"


typedef struct
{
	size_t size;
	char** documents;
} Documents;


typedef struct
{
	char* word;
	List* doc;
} Word;


typedef struct
{
	size_t docID;
	List* index;
} Doc;


Documents* create_documents(FILE* stream);
char* getString(FILE* stream);
size_t getWordList(Btree** btree, Documents* documents);
size_t getWords(FILE* stream, Btree** btree, size_t fileID);
size_t handle_word(Btree** btree, char* word, size_t doc_id, size_t index);
size_t* create_index(size_t index);
Doc* create_doc(size_t docID, size_t* index);
Word* create_word(char* word, Doc* doc);
void* copyDocID(void* doc);
void freeWord(void* info);
void freeDoc(void* info);
void freeDocuments(Documents* documents);
void toUpperCase(char* str);
int compareDoc(void* a, void* b);
void* copyIndex(void* index);
void* copyDoc(void* doc);
void mountDocuments(FILE* stream, Documents** documents);
void flushDocuments(FILE* stream, Documents* documents);
void* mountWord(FILE* stream);
void flushWord(FILE* stream, void* w);
wchar_t* ascii2wchart(const char* string);
int cmpKey(void* k1, void* k2);
int cmpStrKey(void* k1, void* k2);

#endif
