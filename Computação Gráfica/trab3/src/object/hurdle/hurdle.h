/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of shooting and improvements on moving.
 *
 */
 
#ifndef HURDLE_H
#define HURDLE_H

#include "../../util/list/list.h"

typedef struct Hurdle_t Hurdle;

Hurdle* new_hurdle(float radius, float cx, float cy, float red, float green, float blue);
void delete_hurdle(void** hurdle);
void display_hurdle(void* hurdle);
int collided_hurdle_list(List* hurdles, float cx, float cy, float radius);
void adjust_hurdle(void* hurdle, void* window);

#endif
