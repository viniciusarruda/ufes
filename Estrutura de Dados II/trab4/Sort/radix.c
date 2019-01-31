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
#include "../IO/io.h"

#define get_memory_zeroed(ptr, n, size) ((ptr) = calloc((n), (size)));if((ptr) == NULL){fprintf(stderr, "Out of memory.\n");exit(EXIT_FAILURE);}


void radix(Number** numbers)
{
	unsigned int i, k, e = 1;
	Number* bucket[10];  /* Minha fila. */
	Number* tail[10];    /* Minha fila. */
	Number* index;

/*
	index = *numbers;
	while(index != NULL)
	{
		if (index->value > maior)
			maior = index->value;
		index = index->next;
	}
*/

	for(k = 0; k < 7; k++, e *= 10)
	{
		for(i = 0; i < 10; i++) bucket[i] = tail[i] = NULL;

		index = *numbers;
		while(index != NULL)
		{
			i = (index->value / e) % 10;
			
			if(tail[i] == NULL)
				bucket[i] = index;
			else
				tail[i]->next = index;

			tail[i] = index;
			index = index->next; 
		}

		for(i = 0; i < 10; i++)
		{
			if(tail[i] != NULL)
			{
				*numbers = bucket[i];
				index = tail[i];
				for(i++; i < 10; i++)   /* Este for foi colocado aqui na tentativa de evitar if's desnecessários. */
				{                       /* O primeiro loop já bastava, mas teria que adicionar um if(index == NULL).  */
					if(tail[i] != NULL)
					{
						index->next = bucket[i];
						index = tail[i];
					}
				}
			}
		}

		index->next = NULL;
	}
}


void radixbin(Number** numbers)
{
	unsigned int mask = 1;
	Number* bucket_z;
	Number* bucket_o;
	Number* tail_z;
	Number* tail_o;
	Number* index;

/*
	index = *numbers;
	while(index != NULL)
	{
		if (index->value > maior)
			maior = index->value;
		index = index->next;
	}
*/

	/*
	 011110100001001000000 << 1.000.000 em binário. | O loop percore até a máscara for igual ao bit mais significativo 
	 100000000000000000000 << 1048576 em binário.   | deste número, logo, while(!(mask & 1048576)).
	*/

	while(!(mask & 1048576))
	{
		bucket_z = bucket_o = tail_z = tail_o = NULL;

		index = *numbers;
		while(index != NULL)
		{	
			if(mask & index->value)
			{
				if(tail_o == NULL)
					bucket_o = index;
				else
					tail_o->next = index;

				tail_o = index;
			}
			else
			{
				if(tail_z == NULL)
					bucket_z = index;
				else
					tail_z->next = index;

				tail_z = index;
			}

			index = index->next; 
		}

		/* Os possíveis loops percorrendo os baldes foram desenrolados. */

		if(tail_z != NULL)
		{
			*numbers = bucket_z;

			if(tail_o == NULL)
			{
				tail_z->next = NULL;
			}
			else
			{
				tail_z->next = bucket_o;
				tail_o->next = NULL;
			}
		}
		else
		{
			*numbers = bucket_o;
			tail_o->next = NULL;
		}
		
		mask <<= 1;
	}
}

