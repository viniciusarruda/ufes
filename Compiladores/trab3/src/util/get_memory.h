#ifndef GET_MEMORY_H
#define GET_MEMORY_H

#include <stdio.h>
#include <stdlib.h>

/*
 * Macro para alocar memória no heap de size bytes para o ponteiro ptr.
 */
#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Error while allocating memory.\n");exit(EXIT_SUCCESS);}

#endif

