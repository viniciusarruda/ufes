/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of an interpreter for the language C-Minus
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "../util/get_memory.h"
#include "../util/error.h"

#define STACK_SIZE 100   /* will be resized automatically if needed */

typedef struct
{
    int sp;
    int fp;
    int size;
    int* stack;
} Stack;

static Stack* stack = NULL;


#ifdef DEBUG_STACK
static void print_stack(void) 
{
    int i;
    printf("#%d STACK: ", stack->sp + 1);
    for (i = 0; i <= stack->sp; i++) 
        printf("%d ", stack->stack[i]);
    printf("\n");
}
#else
#define print_stack()
#endif


void new_stack(void)
{
    get_memory(stack, sizeof(Stack));
    get_memory(stack->stack, sizeof(int) * STACK_SIZE);    
    stack->size = STACK_SIZE;
    stack->sp = -1;
    stack->fp = 0;
    print_stack();
}


void delete_stack(void)
{
    print_stack();
    free(stack->stack);
    free(stack);
    stack = NULL;
}


void push_fp(void)
{
    push(stack->fp);
    stack->fp = stack->sp;
}


void pop_fp(void)
{
    stack->fp = pop();
}


int get_next_address(void)
{
    return stack->sp - stack->fp + 1;  
}


int get_fp(void)
{
    return stack->fp; 
}


void push_times(int x, int n)
{
    for(; n > 0; n--)
        push(x); 
}


static void resize(void)
{
    stack->size += STACK_SIZE;
    get_more_memory(stack->stack, sizeof(int) * stack->size);
}


int push(int x) 
{
    if(++stack->sp >= stack->size)
        resize();

    stack->stack[stack->sp] = x;

    print_stack();

    return stack->sp - stack->fp;  /* return offset (by fp) where was pushed */
}


void pop_times(int n)
{
    for(; n > 0; n--)
        pop(); 
}


int pop(void) 
{
    int value = stack->stack[stack->sp--];
    print_stack();
    return value;
}


int load(int offset)
{
	return stack->stack[stack->fp + offset];
}


void store(int offset, int value)
{
	stack->stack[stack->fp + offset] = value;
    print_stack();
}



