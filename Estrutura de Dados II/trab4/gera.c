/*
 * Aluno: Vinicius Ferraço Arruda 
 * Curso: Ciência da Computação
 * E-mail: viniciusferracoarruda@gmail.com 
 * Disciplina: Estrutura de Dados II
 * Professor: Mariella Berger
 * Data: 27 de novembro de 2015
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static void aleatorio(unsigned int n);
static void crescente(unsigned int n);
static void decrescente(unsigned int n);


#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Out of memory.\n");exit(EXIT_FAILURE);}

int main(int argc, char** argv)
{	
	int n;

	if(argc != 3)
		fprintf(stdout, "Usage: %s -[a c d] n\n", argv[0]);
	else if((n = atoi(argv[2])) <= 0)
		fprintf(stdout, "n cannot be negative or zero.\n");
	else if(argv[1][1] == 'a')
		aleatorio((unsigned int) n);
	else if(argv[1][1] == 'c')
		crescente((unsigned int) n);
	else if(argv[1][1] == 'd')
		decrescente((unsigned int) n);
	else 
		fprintf(stdout, "Usage: %s -[a c d] n\n", argv[0]);

	return 0;
}


int cmp_c(const void* a, const void* b)
{
	const unsigned int* x = a;
	const unsigned int* y = b;

	return (*x < *y) ? -1 : (*x == *y) ? 0 : 1;
}


int cmp_d(const void* a, const void* b)
{
	const unsigned int* x = a;
	const unsigned int* y = b;

	return (*x < *y) ? 1 : (*x == *y) ? 0 : -1;
}


static void aleatorio(unsigned int n)
{
	unsigned int i;
	unsigned int* numbers;

	get_memory(numbers, n * sizeof(unsigned int));

	srand((unsigned int) time(NULL));
	
	for(i = 0; i < n; i++)
	{
		numbers[i] = (unsigned int) rand() % 1000001;
	}

	for(i = 0; i < n; i++)
	{
		printf("%u\n", numbers[i]);
	}

	free(numbers);
}

static void crescente(unsigned int n)
{
	unsigned int i;
	unsigned int* numbers;

	get_memory(numbers, n * sizeof(unsigned int));

	srand((unsigned int) time(NULL));
	
	for(i = 0; i < n; i++)
	{
		numbers[i] = (unsigned int) rand() % 1000001;
	}

	qsort(numbers, n, sizeof(unsigned int), cmp_c);

	for(i = 0; i < n; i++)
	{
		printf("%u\n", numbers[i]);
	}

	free(numbers);
}


static void decrescente(unsigned int n)
{
	unsigned int i;
	unsigned int* numbers;

	get_memory(numbers, n * sizeof(unsigned int));

	srand((unsigned int) time(NULL));
	
	for(i = 0; i < n; i++)
	{
		numbers[i] = (unsigned int) rand() % 1000001;
	}

	qsort(numbers, n, sizeof(unsigned int), cmp_d);

	for(i = 0; i < n; i++)
	{
		printf("%u\n", numbers[i]);
	}

	free(numbers);
}

