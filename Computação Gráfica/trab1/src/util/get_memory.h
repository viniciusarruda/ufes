/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */

#ifndef GET_MEMORY_H
#define GET_MEMORY_H

#include <stdio.h>
#include <stdlib.h>

#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Error while allocating memory.\n");exit(EXIT_SUCCESS);}
#define get_more_memory(ptr, size) ((ptr) = realloc((ptr), (size)));if((ptr) == NULL){fprintf(stderr, "Error while reallocating memory.\n");exit(EXIT_SUCCESS);}

/* implementar um free memory */

#endif

