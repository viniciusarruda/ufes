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
#include <stddef.h>   /* wchar_t */
#include <locale.h>   /* setlocale */
#include <string.h>   /* strdup */
#include <wchar.h>    /* wide caracters functions */
#include <wctype.h>   /* towupper */
#include <limits.h>   /* UINT_MAX */
#include <ctype.h>
#include "../GenericList/GenericList.h"
#include "../Word/word.h"
#include "../Btree/Btree.h"


/* Flag para identificar se é uma query com áspas ou sem áspas. */
typedef enum {NONQUOTES, QUOTES} Q_TYPE;


/* Estrutura de uma query. */
typedef struct 
{
	Q_TYPE type;
	wchar_t* original;
	List* query;
} Query;


static void printDocuments(void* doc_elem, void* documents);
static int seqEquals(void* b, void* a);
static int sequencial(void* b, void* a);


/*
 * Faz a pesquisa das queries na estrutura haystack.
 */
void runSearch(List* queries, Documents* documents, void* haystack, Word* (* search) (void *, char *))
{
	Query* query;
	List* query_words;
	List* doc_query;
	Word* word;
	char* str;
	char *old_locale, *saved_locale;


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


	while((query = removeBegList(&queries)) != NULL)
	{
		query_words = query->query;
		fwprintf(stdout, L"%ls\n", query->original);
		

		if((str = removeBegList(&query_words)) != NULL)
		{
			word = search(haystack, str);
			free(str);

			if(word != NULL)
			{
				doc_query = copyList(word->doc, copyDoc);

				while((str = removeBegList(&query_words)) != NULL)
				{
					word = search(haystack, str);
					free(str);

					if(word == NULL)
					{
						freeList(&doc_query, freeDoc);
						freeList(&query_words, free);
						break;
					}


					if(query->type == QUOTES)
						intersection(&doc_query, word->doc, seqEquals, freeDoc);
					else
						intersection(&doc_query, word->doc, compareDoc, freeDoc);


					if(emptyList(doc_query))
					{
						freeList(&query_words, free);
						break;
					}
				}

				if(!emptyList(doc_query))
					backpropagation(&doc_query, documents, printDocuments);
			}
			else
			{
				freeList(&query_words, free);
			}

		}
		else
		{
			freeList(&query_words, free);
		}
	
		free(query->original);
		free(query);
	}


	/* Restore the original locale. */
	setlocale (LC_ALL, saved_locale);
	free (saved_locale);
}


/*
 * Faz a pesquisa das queries na árvore B.
 */
void runBtreeSearch(List* queries, Documents* documents, Btree* haystack, int (* cmp) (void *, void *))
{
	Query* query;
	List* query_words;
	List* doc_query;
	Word* word;
	char* str;
	char *old_locale, *saved_locale;


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


	while((query = removeBegList(&queries)) != NULL)
	{
		query_words = query->query;
		fwprintf(stdout, L"%ls\n", query->original);
		

		if((str = removeBegList(&query_words)) != NULL)
		{
			word = (Word *) binarySearch(haystack, str, cmp);
			free(str);

			if(word != NULL)
			{
				doc_query = copyList(word->doc, copyDoc);

				while((str = removeBegList(&query_words)) != NULL)
				{
					word = (Word *) binarySearch(haystack, str, cmp);
					free(str);

					if(word == NULL)
					{
						freeList(&doc_query, freeDoc);
						freeList(&query_words, free);
						break;
					}


					if(query->type == QUOTES)
						intersection(&doc_query, word->doc, seqEquals, freeDoc);
					else
						intersection(&doc_query, word->doc, compareDoc, freeDoc);


					if(emptyList(doc_query))
					{
						freeList(&query_words, free);
						break;
					}
				}

				if(!emptyList(doc_query))
					backpropagation(&doc_query, documents, printDocuments);
			}
			else
			{
				freeList(&query_words, free);
			}

		}
		else
		{
			freeList(&query_words, free);
		}
	
		free(query->original);
		free(query);
	}


	/* Restore the original locale. */
	setlocale (LC_ALL, saved_locale);
	free (saved_locale);
}


/* 
 * Retorna uma lista de query do arquivo stream.
 */
List* getQueries(FILE* stream)
{
	wchar_t* bufferw;
	char* buffer;
	char* temp;
	wchar_t* tempw;
	char* minimal;
    unsigned int capacity;
    unsigned int n;
    unsigned int capacityw;
    unsigned int nw;
    wint_t wc;
    char c;
	Query* new;
	List* query;
	List* end_query;
	List* queries;
	List* end_queries;
	Q_TYPE type;
	char *old_locale, *saved_locale;


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


	buffer = NULL;
	capacity = 0;
	bufferw = NULL;
	capacityw = 0;

	queries = createEmptyList();


	do
	{
		type = NONQUOTES;
		query = createEmptyList();
		nw = 0;

		while(1)
		{
			n = 0;

			while((wc = fgetwc(stream)) != WEOF && iswalnum(wc))
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

		        if (nw + 1 > capacityw)
		        {
		            if (capacityw == 0)
		                capacityw = 32;
		            else if (capacityw <= (UINT_MAX / 2))
		                capacityw *= 2;
		            else
		            {
		                free(bufferw);
		                return NULL;
		            }

		            tempw = realloc(bufferw, capacityw * sizeof(wchar_t));
		            if (tempw == NULL)
		            {
		                free(bufferw);
		                return NULL;
		            }
		            bufferw = tempw;
		        }


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


		        buffer[n++] = c;
		        bufferw[nw++] = (wchar_t) wc;
			}

			if(wc != L'\n' && wc != WEOF)
				bufferw[nw++] = (wchar_t) wc;

			if(n != 0)
			{
				minimal = malloc((n + 1) * sizeof(char));
			    strncpy(minimal, buffer, n);
    			minimal[n] = '\0';
				insertEndList(&query, &end_query, minimal);
			}


			if(wc == L'"' && n == 0)
			{
				type = QUOTES;
			}
			else if(wc == L'"' && type == QUOTES)
			{
				while((wc = fgetwc(stream)) != WEOF && wc != L'\n');
				break;
			}
			else if(wc == L'\n' || wc == WEOF)
			{
				break;
			}
		}

		if(!emptyList(query))
		{
			new = (Query *) malloc(sizeof(Query));

			if(new == NULL)
			{
				fprintf(stderr, "Out of memory.\n");
				exit(EXIT_FAILURE);
			}

			tempw = malloc((nw + 1) * sizeof(wchar_t));
			wcsncpy(tempw, bufferw, nw);
			tempw[nw] = L'\0';

			new->type = type;
			new->original = tempw;
			new->query = query;

			insertEndList(&queries, &end_queries, new);
		}
	}
	while(wc != WEOF);


	if(capacity > 0)
		free(buffer);

	if(capacityw > 0)
		free(bufferw);

	/* Restore the original locale. */
	setlocale (LC_ALL, saved_locale);
	free (saved_locale);

	return queries;
}


static void printDocuments(void* doc_elem, void* documents)
{
	wchar_t* ws;
	Doc* d = (Doc *) doc_elem;
	Documents* doc = (Documents *) documents;

	ws = ascii2wchart(doc->documents[d->docID]);

	if(ws == NULL)
	{
		fprintf(stderr, "Error converting char* to wchar_t*.\n");
		exit(EXIT_FAILURE);
	}

	fwprintf(stdout, L"%ls\n", ws);

	free(ws);
	freeDoc(d);
}


/* Identifica se um valor é sequencial de outro. */
static int sequencial(void* b, void* a)
{
	size_t* s_a = a;
	size_t* s_b = b;

	if(*s_a + 1 == *s_b)
	{
		*s_a += 1;
		return 1;
	}

	return 0;
}


/* Identifica se um documento possue palavras em sequencia. */
static int seqEquals(void* b, void* a)
{
	Doc* d_a = a;
	Doc* d_b = b;

	if(d_a->docID == d_b->docID)
	{
		intersection(&d_a->index, d_b->index, sequencial, free);

		if(!emptyList(d_a->index))
			return 1;
	}

	return 0;
}


