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
#include <unistd.h>
#include <string.h>
#include <time.h>

#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Out of memory.\n");exit(EXIT_FAILURE);}
#define get_memory_zeroed(ptr, n, size) ((ptr) = calloc((n), (size)));if((ptr) == NULL){fprintf(stderr, "Out of memory.\n");exit(EXIT_FAILURE);}

/*
https://en.wikipedia.org/wiki/Bubble_sort

procedure bubbleSort( A : list of sortable numberss )
    n = length(A)
    repeat
       newn = 0
       for i = 1 to n-1 inclusive do
          if A[i-1] > A[i] then
             swap(A[i-1], A[i])
             newn = i
          end if
       end for
       n = newn
    until n = 0
end procedure;

*/
void bubble(unsigned int* numbers, unsigned int n)
{
    unsigned int i, k, tmp;

    do
    {
        k = 0;
        for(i = 1; i < n; i++)
        {
            if(numbers[i - 1] > numbers[i])
            {
                tmp = numbers[i];
                numbers[i] = numbers[i - 1];
                numbers[i - 1] = tmp;
                k = i; /* Utilizar não só uma flag (true or false), mas isso ! Mais eficiente ! */
            }
        }
        n = k;
    }
    while(n != 0);
}


/* http://www.ufpa.br/sampaio/curso_de_estdados_2/aula2_2.htm */
void shake(unsigned int* numbers, unsigned int n)
{
	long int i, k, l, r;
	unsigned int tmp;

	l = 1;
	r = n - 1;
	k = n - 1;

	do
	{
		for(i = r; i >= l; i--)
		{
			if(numbers[i - 1] > numbers[i])
			{
	            tmp = numbers[i - 1];
	            numbers[i - 1] = numbers[i];
	            numbers[i] = tmp;
	            k = i;
	        }
	    }

	    l = k + 1;

	    for(i = l; i <= r; i++)
	    {
	    	if(numbers[i - 1] > numbers[i])
	        {
	            tmp = numbers[i - 1];
	            numbers[i - 1] = numbers[i];
	            numbers[i] = tmp;
	            k = i;
	        }
	    }

	    r = k - 1;
	}	
	while(l <= r);
}


/* http://rosettacode.org/wiki/Sorting_algorithms/Insertion_sort#C */
void insertion(unsigned int* numbers, unsigned int n)
{
    unsigned int i, j, t;

    for (i = 1; i < n; i++)
    {
        t = numbers[i];
        for (j = i; j > 0 && t < numbers[j - 1]; j--) 
        {
            numbers[j] = numbers[j - 1];
        }
        numbers[j] = t;
    }
}


/* http://rosettacode.org/wiki/Sorting_algorithms/Shell_sort#C */
void shell(unsigned int* numbers, unsigned int n)
{
    unsigned int h, i, j, t;

    for (h = n; h /= 2;)
    {
        for (i = h; i < n; i++)
        {
            t = numbers[i];
            for (j = i; j >= h && t < numbers[j - h]; j -= h)
            {
                numbers[j] = numbers[j - h];
            }
            numbers[j] = t;
        }
    }
}


/* http://rosettacode.org/wiki/Sorting_algorithms/Selection_sort#C */
void selection(unsigned int* numbers, unsigned int n)
{
    unsigned int i, j, m, tmp;

    for (i = 0; i < n; i++)
    {
        for (j = i, m = i; j < n; j++)
        {
            if (numbers[j] < numbers[m])
            {
                m = j;
            }
        }

        tmp = numbers[i];
        numbers[i] = numbers[m];
        numbers[m] = tmp;
    }
}


/* http://rosettacode.org/wiki/Sorting_algorithms/Merge_sort#C */
static void merge_rec(unsigned int* numbers, unsigned int n, unsigned int m) 
{
    unsigned int i, j, k;
    unsigned int* tmp;

    get_memory(tmp, (n * sizeof(unsigned int)));

    for (i = 0, j = m, k = 0; k < n; k++)
    {
        tmp[k] = j == n      ? numbers[i++]
               : i == m      ? numbers[j++]
               : numbers[j] < numbers[i] ? numbers[j++]
               :               numbers[i++];
    }

    for (i = 0; i < n; i++)
    {
        numbers[i] = tmp[i];
    }

    free(tmp);
}

/* http://rosettacode.org/wiki/Sorting_algorithms/Merge_sort#C */
void merge(unsigned int* numbers, unsigned int n)
{
    unsigned int m;

    if (n < 2)
        return;

    m = n / 2;
    merge(numbers, m);
    merge(numbers + m, n - m);
    merge_rec(numbers, n, m);
}


/*http://www.geeksforgeeks.org/iterative-quick-sort/*/
void quick_random(unsigned int* numbers, int left, int right) 
{
	int i, j, pivot;  
    unsigned int p, t;

    if (left < right)
    {
        i = left + 1; /* + 1 devido ao pivo */
        j = right;

        pivot = left + (rand() % (right - left + 1));
        p = numbers[pivot], numbers[pivot] = numbers[left], numbers[left] = p;

        while (1) 
        {                                    
            while (i <= right && numbers[i] <= p) ++i;            
            while (p < numbers[j]) --j;            

            if (i >= j) break;                       

            t = numbers[i], numbers[i] = numbers[j], numbers[j] = t;            

            ++i; --j;                                   
        }

        numbers[left] = numbers[j], numbers[j] = p; 

        quick_random(numbers, left, j - 1);
        quick_random(numbers, j + 1, right);                   
    }                                     
}

void quickrandom(unsigned int* numbers, unsigned int n) 
{
    srand((unsigned int) time(NULL));
    quick_random(numbers, 0, (int) n - 1);
}


/* http://www.ime.usp.br/~pf/algoritmos/aulas/quick.html */
void quick_central(unsigned int* numbers, int left, int right) 
{
	int i, j, pivot;  
    unsigned int p, t;

    if (left < right)
    {
        i = left + 1; /* + 1 devido ao pivo */
        j = right;

        pivot = left + (right - left) / 2;
        p = numbers[pivot], numbers[pivot] = numbers[left], numbers[left] = p;

        while (1) 
        {                                    
            while (i <= right && numbers[i] <= p) ++i;            
            while (p < numbers[j]) --j;            

            if (i >= j) break;                       

            t = numbers[i], numbers[i] = numbers[j], numbers[j] = t;            

            ++i; --j;                                   
        }

        numbers[left] = numbers[j], numbers[j] = p; 

        quick_central(numbers, left, j - 1);
        quick_central(numbers, j + 1, right);                   
    }                                     
}

void quickcentral(unsigned int* numbers, unsigned int n) 
{
    quick_central(numbers, 0, (int) n - 1);
}




void quick_primeiro(unsigned int* numbers, int left, int right) 
{
	int i, j;  
    unsigned int p, t;

    if (left < right)
    {
        i = left + 1; /* + 1 devido ao pivo */
        j = right;

        p = numbers[left];

        while (1) 
        {                                    
            while (i <= right && numbers[i] <= p) ++i;            
            while (p < numbers[j]) --j;            

            if (i >= j) break;                       

            t = numbers[i], numbers[i] = numbers[j], numbers[j] = t;            

            ++i; --j;                                   
        }

        numbers[left] = numbers[j], numbers[j] = p; 

        quick_primeiro(numbers, left, j - 1);
        quick_primeiro(numbers, j + 1, right);                   
    }                                     
}

void quickprimeiro(unsigned int* numbers, unsigned int n) 
{
    quick_primeiro(numbers, 0, (int) n - 1);
}



void quick_mediana3(unsigned int* numbers, int left, int right) 
{
	int i, j, pivot;  
    unsigned int p, t;

    if (left < right)
    {
        i = left + 1; /* + 1 devido ao pivo */
        j = right;

        pivot = left + (right - left) / 2;
        
        /* Calculo da mediana de tres. */
	    if(numbers[left] < numbers[pivot]) { if(numbers[pivot] > numbers[right]) pivot = right;}
	    else if(numbers[right] < numbers[pivot]) { if(numbers[pivot] > numbers[left]) pivot = left;}
	    else { if(numbers[left] < numbers[right]) pivot = left; else pivot = right;}

        p = numbers[pivot], numbers[pivot] = numbers[left], numbers[left] = p;

        while (1) 
        {                                    
            while (i <= right && numbers[i] <= p) ++i;            
            while (p < numbers[j]) --j;            

            if (i >= j) break;                       

            t = numbers[i], numbers[i] = numbers[j], numbers[j] = t;            

            ++i; --j;                                   
        }

        numbers[left] = numbers[j], numbers[j] = p; 

        quick_mediana3(numbers, left, j - 1);
        quick_mediana3(numbers, j + 1, right);                   
    }                                     
}

void quickmediana3(unsigned int* numbers, unsigned int n) 
{
    quick_mediana3(numbers, 0, (int) n - 1);
}


/* https://pt.wikipedia.org/wiki/Heapsort */
void heap(unsigned int* numbers, unsigned int n) 
{
	unsigned int i = n / 2, pai, filho, t;

	for (;;) 
	{
		if (i > 0) 
		{
			i--;
			t = numbers[i];
		} 
		else 
		{
			n--;
			if (n == 0) return;
			t = numbers[n];
			numbers[n] = numbers[0];
		}

		pai = i;
		filho = i * 2 + 1;

		while (filho < n) 
		{
			if ((filho + 1 < n)  &&  (numbers[filho + 1] > numbers[filho])) filho++;
			if (numbers[filho] > t) 
			{
				numbers[pai] = numbers[filho];
				pai = filho;
				filho = pai * 2 + 1;
			} 
			else 
			{
				break;
			}
		}
		numbers[pai] = t;
	}
}

/* http://pt.slideshare.net/shimulsakhawat/counting-sortnon-comparison-sort */
/* http://www.geeksforgeeks.org/counting-sort/ */
void rank(unsigned int* numbers, unsigned int n) 
{
	unsigned int i, max;
	unsigned int* tmp;

	max = 1000000;
	
	/*
	for(i = 0, max = 0; i < n; i++)
		if(numbers[i] > max)
			max = numbers[i];
	*/
		
	get_memory_zeroed(tmp, max + 1, sizeof(unsigned int));

	for (i = 0; i < n; i++)
	{	
		tmp[numbers[i]]++;
	}

	for(i = 0; i <= max; i++)
	{
		while(tmp[i] > 0)
		{
			*numbers = i;
			numbers++;
			tmp[i]--;
		}
	}

	free(tmp);
}


