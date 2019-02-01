/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a player and hurdles with collision detection and jump effect.
 *
 */
 
#include "../../util/util.h"
#include "../../window/window.h"
#include "../circle/circle.h"
#include "arena.h"
#include <stdio.h>

struct Arena_t
{
	float inner_radius;
	float inner_cx;
	float inner_cy;
	float inner_red;
	float inner_green;
	float inner_blue;

	float outer_radius;
	float outer_cx;
	float outer_cy;
	float outer_red;
	float outer_green;
	float outer_blue;
};

void delete_arena(void** arena)
{
	free_memory(&cast_twice(arena, Arena));
}

void display_arena(void* arena)
{
	display_circle(cast(arena, Arena)->outer_radius, cast(arena, Arena)->outer_cx, cast(arena, Arena)->outer_cy, cast(arena, Arena)->outer_red, cast(arena, Arena)->outer_green, cast(arena, Arena)->outer_blue);
	display_circle(cast(arena, Arena)->inner_radius, cast(arena, Arena)->inner_cx, cast(arena, Arena)->inner_cy, cast(arena, Arena)->inner_red, cast(arena, Arena)->inner_green, cast(arena, Arena)->inner_blue);
}

Arena* new_arena(float inner_radius, float inner_cx, float inner_cy, float inner_red, float inner_green, float inner_blue,
	             float outer_radius, float outer_cx, float outer_cy, float outer_red, float outer_green, float outer_blue)
{
	Arena* new;
	get_memory(new, sizeof(Arena));
	
	new->inner_radius = inner_radius;
	new->inner_cx = inner_cx;
	new->inner_cy = inner_cy;
	new->inner_red = inner_red;
	new->inner_green = inner_green;
	new->inner_blue = inner_blue;

	new->outer_radius = outer_radius;
	new->outer_cx = outer_cx;
	new->outer_cy = outer_cy;
	new->outer_red = outer_red;
	new->outer_green = outer_green;
	new->outer_blue = outer_blue;

	return new;
}

void adjust_arena(Arena* arena, Window* window)
{
	arena->inner_cy = adjust_y(window, arena->inner_cy);
	arena->outer_cy = adjust_y(window, arena->outer_cy);
}

float get_arena_size(Arena* arena)
{
	return arena->outer_radius;
}

float get_arena_cx(Arena* arena)
{
	return arena->outer_cx;
}

float get_arena_cy(Arena* arena)
{
	return arena->outer_cy;
}

int collided_arena(Arena* arena, float cx, float cy, float radius)
{
	return collided_circle(arena->inner_cx, arena->inner_cy, arena->inner_radius, cx, cy, radius) 
	       || !is_inside(cx, cy, radius, arena->outer_cx, arena->outer_cy, arena->outer_radius);
}
