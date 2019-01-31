/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 12 de outubro de 2015
 */


#include <stddef.h>   /* wchar_t */
#include <locale.h>   /* setlocale */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>   /* strdup */
#include <wchar.h>    /* wide caracters functions */
#include <wctype.h>   /* towupper */
#include <limits.h>   /* UINT_MAX */
#include <ctype.h>

#include "../GenericList/GenericList.h"
#include "../Btree/Btree.h"
#include "word.h"


#define BUFFBYTES 128


static int compare_doc(void* a, void* b);


/*
 * A seguir, funções que comparam documentos e chaves. 
 * São várias pois são utilizadas como função de callback e na implementação utilizam tipo void*.
 */
int cmpKey(void* k1, void* k2)
{
	Word* key1 = (Word *) k1;
	Word* key2 = (Word *) k2;

	return strcmp(key1->word, key2->word); 
}


int cmpStrKey(void* k1, void* k2)
{
	Word* key2 = (Word *) k2;

	return strcmp((char *) k1, key2->word); 
}


static int compare_doc(void* a, void* b)
{
	Doc* key_a = (Doc *) a;
	size_t* key_b = (size_t *) b;
	
	return key_a->docID == *key_b;
}


int compareDoc(void* a, void* b)
{
	Doc* doc_a = (Doc *) a;
	Doc* doc_b = (Doc *) b;

	return doc_a->docID == doc_b->docID;
}


size_t* create_index(size_t index) 
{
	size_t* i;

	i = (size_t *) malloc(sizeof(size_t));
		
	if(i == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	*i = index;
	
	return i;
}


Doc* create_doc(size_t docID, size_t* index)
{
	Doc* d;

	d = (Doc *) malloc(sizeof(Doc));
		
	if(d == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	d->docID = docID;
	
	d->index = createEmptyList();
	insertBegList(&d->index, index);

	return d;
}


Word* create_word(char* word, Doc* doc)
{
	Word* w;
	
	w = (Word *) malloc(sizeof(Word));
		
	if(w == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}
	
	w->word = word;
	
	w->doc = createEmptyList();
	insertBegList(&w->doc, doc);
	
	return w;
}


void freeWord(void* info)
{
	Word* w = (Word *) info;

	free(w->word);
	freeList(&w->doc, freeDoc);
	free(w);
}


void freeDoc(void* info)
{
	Doc* d = (Doc *) info;

	freeList(&d->index, free);
	free(d);
}


Documents* create_documents(FILE* stream)
{
	List* file_list;
	char* str;
	size_t i; 
	Documents* documents;

	
	documents = (Documents *) malloc(sizeof(Documents));
	
	if(documents == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	file_list = createEmptyList();
	
	documents->size = 0;
	
	while((str = getString(stream)) != NULL)
	{
		insertBegList(&file_list, str);
		documents->size++;
	}

	documents->documents = (char **) malloc(documents->size * sizeof(char *));
	
	if(documents->documents == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < documents->size; i++)
	{
		documents->documents[i] = (char *) removeBegList(&file_list);
	}

	return documents;
}


void freeDocuments(Documents* documents)
{
	size_t i;

	for(i = 0; i < documents->size; i++)
	{
		free(documents->documents[i]);
	}

	free(documents->documents);
	free(documents);
}


char* getString(FILE* stream)
{
    char* buffer;
	char* temp;
	char* minimal;
    unsigned int capacity;
    unsigned int n;
    int c;

	buffer = NULL;
	capacity = 0;
	n = 0;
	

    while ((c = fgetc(stream)) != '\n' && c != EOF)
    {
        if (n + 1 > capacity)
        {
            if (capacity == 0)
                capacity = 32;
            else if (capacity <= (UINT_MAX / 2))
                capacity *= 2;
            else
            {
                free(buffer);
                return NULL;
            }

            temp = realloc(buffer, capacity * sizeof(char));
            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }

        buffer[n++] = (char) c;
    }

    /* return NULL if user provided no input */
    if (n == 0)
        return NULL;

    /* minimize buffer */
    minimal = malloc((n + 1) * sizeof(char));
    strncpy(minimal, buffer, n);
    free(buffer);

    /* terminate string */
    minimal[n] = '\0';

    return minimal;
}


/*
 * Retorna, junto à função auxiliar getWords, a lista de palavras dos arquivos indexadas em uma árvore B.
 */ 
size_t getWordList(Btree** btree, Documents* documents)
{
	size_t i, n_words;
	FILE* stream;
	
	n_words = 0;
	*btree = new();

	for(i = 0; i < documents->size; i++)
	{
		stream = fopen(documents->documents[i], "r");
		
		if(stream == NULL)
		{
			fprintf(stderr, "Error while opening file [%s].\n", documents->documents[i]); 
			exit(EXIT_FAILURE);
		}

		n_words += getWords(stream, btree, i); 

		fclose(stream);
	}

	return n_words;
}


size_t getWords(FILE* stream, Btree** btree, size_t fileID)
{
	wint_t wc;
	char* buff;
	char c;
	size_t buffIndex, buffLen, fileIndex, n_words;
	char *old_locale, *saved_locale;

	fileIndex = n_words = 0;

	buffLen = BUFFBYTES; 

	buff = (char *) calloc(buffLen, sizeof(char));

	if(buff == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	/* Get the name of the current locale.  */
	old_locale = setlocale(LC_ALL, NULL);

	/* Copy the name so it won’t be clobbered by setlocale. */
	saved_locale = strdup(old_locale);
	if (saved_locale == NULL)
	{
	    fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	/* Now change the locale and do some stuff with it. */
	setlocale(LC_ALL, "");


	do
	{
		buffIndex = 0;
	
		while((wc = fgetwc(stream)) != WEOF && iswalnum(wc))
		{
			switch(wc) 
			{
				case L'a': case L'A': case L'À': case L'Á': case L'Â': case L'Ã': case L'Ä': case L'Å': case L'à': case L'á': case L'â': case L'ã': case L'ä': case L'å': c = 'A'; break;
				case L'b': case L'B': c = 'B'; break;
				case L'c': case L'C': case L'Ç': case L'ç': c = 'C'; break;
				case L'd': case L'D': c = 'D'; break;
				case L'e': case L'E': case L'È': case L'É': case L'Ê': case L'Ë': case L'è': case L'é': case L'ê': case L'ë': c = 'E'; break;
				case L'f': case L'F': c = 'F'; break;
				case L'g': case L'G': c = 'G'; break;
				case L'h': case L'H': c = 'H'; break;
				case L'i': case L'I': case L'Ì': case L'Í': case L'Î': case L'Ï': case L'ì': case L'í': case L'î': case L'ï': c = 'I'; break;
				case L'j': case L'J': c = 'J'; break;
				case L'k': case L'K': c = 'K'; break;
				case L'l': case L'L': c = 'L'; break;
		        case L'm': case L'M': c = 'M'; break;
				case L'N': case L'Ñ': case L'ñ': case L'n': c = 'N'; break;
				case L'O': case L'Ò': case L'Ó': case L'Ô': case L'Õ': case L'Ö': case L'ò': case L'ó': case L'ô': case L'õ': case L'ö': case L'o': c = 'O'; break;
				case L'p': case L'P': c = 'P'; break;
				case L'q': case L'Q': c = 'Q'; break;
				case L'r': case L'R': c = 'R'; break;
				case L's': case L'S': c = 'S'; break;
				case L't': case L'T': c = 'T'; break;
				case L'U': case L'Ù': case L'Ú': case L'Û': case L'Ü': case L'ù': case L'ú': case L'û': case L'ü': case L'u': c = 'U'; break;
				case L'v': case L'V': c = 'V'; break;
				case L'w': case L'W': c = 'W'; break;
				case L'x': case L'X': c = 'X'; break;
				case L'Y': case L'Ý': case L'ý': case L'ÿ': case L'y': c = 'Y'; break;
				case L'z': case L'Z': c = 'Z'; break;

				case L'0': c = '0'; break;
				case L'1': c = '1'; break;
				case L'2': c = '2'; break;
				case L'3': c = '3'; break;
				case L'4': c = '4'; break;
				case L'5': c = '5'; break;
				case L'6': c = '6'; break;
				case L'7': c = '7'; break;
				case L'8': c = '8'; break;
				case L'9': c = '9'; break;

		    }
	
			if(buffIndex == buffLen - 1)
			{			
				buffLen += BUFFBYTES;
				buff = (char *) realloc(buff, buffLen);    

				if(buff == NULL)                                                      
				{
					fprintf(stderr, "Out of memory.\n");                   
					exit(EXIT_FAILURE); 										      
				}
			}

			buff[buffIndex++] = c; 
			
		}
	 	buff[buffIndex] = '\0';

	 	if(buffIndex != 0)
			n_words += handle_word(btree, buff, fileID, fileIndex);


		if(wc == WEOF)
		 	break;

		while((wc = fgetwc(stream)) != WEOF && !iswalnum(wc));

		if(wc == WEOF)
		 	break;

		ungetwc(wc, stream); 

		fileIndex++;
	}
	while(wc != WEOF);


	/* Restore the original locale. */
	setlocale (LC_ALL, saved_locale);
	free (saved_locale);
 	
 	free(buff);

 	return n_words;
}


/*
 * Busca a palavra na árvore, caso não estiver presente, insere ela. Caso estiver presente na árvore, 
 * é inserido na lista de documentos da palavra existente o documento atual da palavra que se deseja inserir.
 * Caso já existirem a palavra e o documento que ela está presente, apenas o indice é inserido.
 * Retorna se 1 se a palavra foi inserida e 0 se a palavra foi atualizada na árvore.
 */
size_t handle_word(Btree** btree, char* word, size_t doc_id, size_t index)
{
	Word* word_in_list;
	Doc* doc_in_list;

	if((word_in_list = (Word *) binarySearch(*btree, word, cmpStrKey)) == NULL)
	{
		word = strdup(word);
		
		if(word == NULL)
		{
			fprintf(stderr, "Out of memory.\n");
			exit(EXIT_FAILURE);
		}

		insert(btree, create_word(word, create_doc(doc_id, create_index(index))), cmpKey);

		return 1;
	}
	else
	{
		if((doc_in_list = searchInfo(word_in_list->doc, &doc_id, compare_doc)) == NULL)
		{
			insertBegList(&word_in_list->doc, create_doc(doc_id, create_index(index)));
		}
		else
		{
			insertBegList(&doc_in_list->index, create_index(index));
		}
	}

	return 0;
}


void* copyDocID(void* doc)
{
	Doc* d = (Doc *) doc;
	return create_index(d->docID);
}


void* copyDoc(void* doc)
{
	Doc* new;
	Doc* d = (Doc *) doc;

	new = (Doc *) malloc(sizeof(Doc));
		
	if(new == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	new->docID = d->docID;
	new->index = copyList(d->index, copyIndex);

	return new;
}


void toUpperCase(char* str)
{
	while(*str)
	{
		*str = (char) toupper((int) *str);
		str++;
	}
}


void* copyIndex(void* index) 
{
	size_t* i;
	size_t* ind = (size_t *) index;

	i = (size_t *) malloc(sizeof(size_t));
		
	if(i == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	*i = *ind;
	
	return i;
}


/*
 * Monta uma lista de documentos onde será feita a indexação.
 */
void mountDocuments(FILE* stream, Documents** documents)
{
	size_t i;

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
}


void flushDocuments(FILE* stream, Documents* documents)
{
	size_t i;

	fprintf(stream, "%lu\n", documents->size);
	for(i = 0; i < documents->size; i++)
	{
		fprintf(stream, "%s\n", documents->documents[i]);
		free(documents->documents[i]);	
	}
	free(documents->documents);
	free(documents);
}


void* mountWord(FILE* stream)
{
	int c;
	char* str;
	Word* word;
	Doc* doc;
	size_t docID;
	size_t index;


	if((str = getString(stream)) == NULL)
		return NULL;
	

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

	return word;
}


void flushWord(FILE* stream, void* w)
{
	Doc* doc;
	size_t* index;
	Word* word = (Word *) w;

	fprintf(stream, "%s\n", word->word);

	while(!emptyList(word->doc))
	{
		doc = (Doc *) removeBegList(&word->doc);

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
	fprintf(stream, "\n");
	free(word->word);
	free(word);
}


/* Converte uma string em ascii para uma string em wchar_t.   */
/* http://www.aquaphoenix.com/ref/gnu_c_library/libc_311.html */
wchar_t* ascii2wchart(const char* string)
{
	size_t size; 
	wchar_t *buf;

	size = strlen (string) + 1;

	buf = malloc(size * sizeof (wchar_t));

	if(buf == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	size = mbstowcs(buf, string, size);

	if (size == (size_t) -1)
	{
		free(buf);
		return NULL;
	}


	buf = realloc (buf, (size + 1) * sizeof (wchar_t));

	if(buf == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		exit(EXIT_FAILURE);
	}

	return buf;
}

