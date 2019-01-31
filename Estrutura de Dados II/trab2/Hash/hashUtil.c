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


/* Retorna o próximo número primo após o valor value. */
size_t ceil_prime(size_t value)
{
	int flag;
	size_t i, j, prime, max_size;

	flag = 1;
	
	max_size = (size_t)-1; /* http://stackoverflow.com/questions/3472311/what-is-a-portable-method-to-find-the-maximum-value-of-size-t */
	prime = max_size;      /* Caso nao encontrar um ceil_prime, fica como o maximo possivel. */
  
	for(i = value; i < max_size; i++)
	{
		flag = 0;
		for(j = 2; j <= i / 2; j++)
		{
			if(i % j == 0)
			{
				flag = 1;
				break;
			}
		}
		
		if(flag == 0)
		{
			prime = i;
			break;
		}
	}
  
	return prime;
}


/* Implementação da djb2: http://www.cse.yorku.ca/~oz/hash.html */
size_t hash_function(char* str)
{
	unsigned long hash = 5381;
	int c;

	c = *str;
	str++;
	while (c)
	{
		hash = hash * 33 + (unsigned long) c;  
		c = *str;
		str++;
	}

	return hash;
}


/* Para a estrutura Rehash. Soma o valores ascii's da string. */
size_t hash_2_function(char* str)
{
	size_t hash = 0;

	if(str == NULL || *str == '\0')
	{
		fprintf(stderr, "String cannot be NULL.\n");
		exit(EXIT_FAILURE);
	}

	while(*str != '\0')
	{
		hash += (size_t) *str;
		str++;
	}

	return hash;
}

