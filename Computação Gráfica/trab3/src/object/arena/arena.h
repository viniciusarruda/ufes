/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of shooting and improvements on moving.
 *
 */
 
#ifndef ARENA_H
#define ARENA_H

#include "../../window/window.h"

typedef struct Arena_t Arena;

Arena* new_arena(float inner_radius, float inner_cx, float inner_cy, float inner_red, float inner_green, float inner_blue,
	             float outer_radius, float outer_cx, float outer_cy, float outer_red, float outer_green, float outer_blue);

float get_arena_size(Arena* arena);
float get_arena_cx(Arena* arena);
float get_arena_cy(Arena* arena);

int collided_arena(Arena* arena, float cx, float cy, float radius);

void delete_arena(void** arena);
void display_arena(void* arena);

void adjust_arena(Arena* arena, Window* window);

#endif
