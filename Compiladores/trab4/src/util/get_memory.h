/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of an interpreter for the language C-Minus
 *
 */

#ifndef GET_MEMORY_H
#define GET_MEMORY_H

#include <stdio.h>
#include <stdlib.h>

#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Error while allocating memory.\n");exit(EXIT_SUCCESS);}
#define get_more_memory(ptr, size) ((ptr) = realloc((ptr), (size)));if((ptr) == NULL){fprintf(stderr, "Error while reallocating memory.\n");exit(EXIT_SUCCESS);}

#endif

