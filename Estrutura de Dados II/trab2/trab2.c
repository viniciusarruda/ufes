/*
 * Aluno: Vinicius Ferraço Arruda
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 12 de outubro de 2015
 */

#include <stdio.h>
#include <string.h>

#include "Word/word.h"
#include "Hash/hashEncadeada.h"
#include "Hash/hashLinear.h"
#include "Hash/rehash.h"
#include "Search/search.h"
#include "Btree/Btree.h"


static void moduloIndexacao(char tipoModelo, FILE* stream_1, FILE* stream_2);
static void moduloBusca(char tipoModelo, FILE* stream_1, FILE* stream_2);


/*
 * main: Confere os argumentos de entrada e se corretos, chama a função com os devidos parâmetros.
 */
int main(int argc, char** argv)
{
	char modulo;
	char tipoModelo;
	FILE* stream_1;
	FILE* stream_2;

	if(argc != 5)
	{
		fprintf(stdout, "Usage: %s [modulo] [tipoModelo] [file name 1] [file name 2]\n", argv[0]);
		return 0;
	}


	if(strlen(argv[1]) == 2 && argv[1][0] == '-' && (argv[1][1] == 'i' || argv[1][1] == 'b'))
	{
		modulo = argv[1][1];
	}
	else
	{
		fprintf(stdout, "Modulo [%s] invalido\n", argv[1]);
		return 0;
	}


	if(strlen(argv[2]) == 1 && (argv[2][0] == 'E' || argv[2][0] == 'L' || argv[2][0] == 'R' || argv[2][0] == 'B'))
	{
		tipoModelo = argv[2][0];
	}
	else
	{
		fprintf(stdout, "Tipo Modelo [%s] invalido\n", argv[2]);
		return 0;
	}


	stream_1 = fopen(argv[3], "r");

	if(stream_1 == NULL)
	{
		fprintf(stdout, "Erro ao abrir arquivo [%s]\n", argv[3]);
		return 0;
	}


	if(modulo == 'i')
	{
		stream_2 = fopen(argv[4], "w");

		if(stream_2 == NULL)
		{
			fprintf(stdout, "Erro ao abrir arquivo [%s]\n", argv[4]);
			return 0;
		}

		moduloIndexacao(tipoModelo, stream_1, stream_2);
	}
	else  
	{	
		stream_2 = fopen(argv[4], "r");

		if(stream_2 == NULL)
		{
			fprintf(stdout, "Erro ao abrir arquivo [%s]\n", argv[4]);
			return 0;
		}
		
		moduloBusca(tipoModelo, stream_1, stream_2);
	}


	fclose(stream_1);
	fclose(stream_2);

	return 0;
}


/*
 * moduloIndexacao: Gera o arquivo indexado de acordo com a estrutura de dados requisitada em tipoModelo.
 */
static void moduloIndexacao(char tipoModelo, FILE* stream_1, FILE* stream_2)
{
	Documents* documents;
	Btree* btree;
	size_t n_words;

	documents = create_documents(stream_1);
	n_words = getWordList(&btree, documents);

	switch(tipoModelo)
	{
		case 'E':
			flushHashEncadeada(stream_2, documents, getHashEncadeada(btree, n_words));
			break;
		case 'L':
			flushHashLinear(stream_2, documents, getHashLinear(btree, n_words));
			break;
		case 'R':
			flushRehash(stream_2, documents, getRehash(btree, n_words));
			break;
		case 'B':
			flushDocuments(stream_2, documents);
			flushBtree(stream_2, btree, flushWord);
			break;
	}
}


/*
 * moduloBusca: Executa a busca sobre um arquivo de indices. A busca é feita remontando o arquivo de indices 
 *              com a estrutura de dados tipoModelo.
 */
static void moduloBusca(char tipoModelo, FILE* stream_1, FILE* stream_2)
{
	Documents* documents;
	void* data_structure = NULL;

	switch(tipoModelo)
	{
		case 'E':
			data_structure = mountHashEncadeada(stream_1, &documents);
			runSearch(getQueries(stream_2), documents, data_structure, searchHashEncadeada);
			freeHashEncadeada(data_structure, freeWord);
			break;
		case 'L':
			data_structure = mountHashLinear(stream_1, &documents);
			runSearch(getQueries(stream_2), documents, data_structure, searchHashLinear);
			freeHashLinear(data_structure, freeWord);
			break;
		case 'R':
			data_structure = mountRehash(stream_1, &documents);
			runSearch(getQueries(stream_2), documents, data_structure, searchRehash);
			freeRehash(data_structure, freeWord);
			break;
		case 'B':
			mountDocuments(stream_1, &documents);
			data_structure = mountBtree(stream_1, mountWord);
			runBtreeSearch(getQueries(stream_2), documents, data_structure, cmpStrKey);
			freeBtree(data_structure, freeWord);
			break;
	}

	freeDocuments(documents);
}

