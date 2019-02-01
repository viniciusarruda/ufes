/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of an interpreter for the language C-Minus
 *
 */

#ifndef STACK_H
#define STACK_H

void new_stack(void);
void delete_stack(void);

int load(int offset);
void store(int offset, int value);

int get_next_address(void);
int get_fp(void);

void push_fp(void);
void pop_fp(void);

int push(int x);
int pop(void);

void pop_times(int n);
void push_times(int x, int n);

#endif
