/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of an interpreter for the language C-Minus
 *
 */

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

/* If this error occurs, let me know with which input the interpreter was tested */
#define IMPLEMENTATION_ERROR "Something very wrong happened !"  

#define raise_error(msg) {printf("%s\n", msg);exit(EXIT_SUCCESS);}

#endif


