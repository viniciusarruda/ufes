/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* COLOR */
#define MIN_COLOR 0.0
#define MAX_COLOR 1.0
#define DARK_GRAY 0.2, 0.2, 0.2
#define is_red(r, g, b)   (((r) == MAX_COLOR) && ((g) == MIN_COLOR) && ((b) == MIN_COLOR))
#define is_green(r, g, b) (((r) == MIN_COLOR) && ((g) == MAX_COLOR) && ((b) == MIN_COLOR))
#define is_blue(r, g, b)  (((r) == MIN_COLOR) && ((g) == MIN_COLOR) && ((b) == MAX_COLOR))
#define is_white(r, g, b) (((r) == MAX_COLOR) && ((g) == MAX_COLOR) && ((b) == MAX_COLOR))
#define is_black(r, g, b) (((r) == MIN_COLOR) && ((g) == MIN_COLOR) && ((b) == MIN_COLOR))


/* CASTING */
#define cast(object, type) ((type *) (object))
#define cast_twice(object, type) (*((type **) (object)))

/* UNUSED */
#define unused(x) ((void)(x))

/* ERROR */
#define raise_error(msg) {fprintf(stderr, "%s\n", msg);exit(EXIT_SUCCESS);}

/* MEMORY */
#define get_memory(ptr, size) ((ptr) = malloc(size));if((ptr) == NULL){fprintf(stderr, "Error while allocating memory.\n");exit(EXIT_SUCCESS);}
#define get_zeroed_memory(ptr, nmemb, size) ((ptr) = calloc(nmemb, size));if((ptr) == NULL){fprintf(stderr, "Error while allocating memory.\n");exit(EXIT_SUCCESS);}
#define get_more_memory(ptr, size) ((ptr) = realloc((ptr), (size)));if((ptr) == NULL){fprintf(stderr, "Error while reallocating memory.\n");exit(EXIT_SUCCESS);}
#define free_memory(ptr) (free(*(ptr))); ((*(ptr)) = NULL);

/* STRING */
#define equal_str(a, b) (strcmp((a), (b)) == 0) 

/* FILE */
#define open_file(stream, opt, file_name) ((stream) = fopen((file_name), (opt))); if((stream) == NULL){fprintf(stderr, "Cannot open file \"%s\"\n", (file_name));exit(EXIT_SUCCESS);}
#define close_file(stream) if(fclose(stream) != 0){fprintf(stderr, "Cannot close file\n");exit(EXIT_SUCCESS);}


/* DEBUG POINT */
#define debug(point, quit) fprintf(stderr, "DEBUG [%s].\n", point); if(quit){exit(EXIT_SUCCESS);}

#endif

