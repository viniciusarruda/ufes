/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of shooting and improvements on moving.
 *
 */
 
#include "../../util/util.h"
#include "../../window/window.h"
#include "../../util/list/list.h"
#include "../../shape/circle/circle.h"
#include <GL/glut.h>
#include "hurdle.h"

struct Hurdle_t
{
	float radius;
	float cx;
	float cy;
	float red;
	float green;
	float blue;
};

Hurdle* new_hurdle(float radius, float cx, float cy, float red, float green, float blue)
{
	Hurdle* new;
	get_memory(new, sizeof(Hurdle));
	new->radius = radius;
	new->cx = cx;
	new->cy = cy;
	new->red = red;
	new->green = green;
	new->blue = blue;
	return new;
}

void adjust_hurdle(void* hurdle, void* window)
{
	cast(hurdle, Hurdle)->cy = adjust_y(window, cast(hurdle, Hurdle)->cy);
}

void delete_hurdle(void** hurdle)  
{
	free_memory(&cast_twice(hurdle, Hurdle));
}

void display_hurdle(void* hurdle)
{
	glPushMatrix();
		
		glTranslatef(cast(hurdle, Hurdle)->cx, cast(hurdle, Hurdle)->cy, 0.0);
		display_circle(cast(hurdle, Hurdle)->radius, cast(hurdle, Hurdle)->red, cast(hurdle, Hurdle)->green, cast(hurdle, Hurdle)->blue);

	glPopMatrix();		
}

int collided_hurdle(Hurdle* hurdle, float cx, float cy, float radius)
{
	return collided_circle(hurdle->cx, hurdle->cy, hurdle->radius, cx, cy, radius);
}

static int collided_hurdle_generic(void* hurdle, void* arg)
{
	float* arg_array = arg;

	return collided_hurdle(cast(hurdle, Hurdle), arg_array[0], arg_array[1], arg_array[2]);
}

int collided_hurdle_list(List* hurdles, float cx, float cy, float radius)
{
	float arg[3];

	arg[0] = cx;
	arg[1] = cy;
	arg[2] = radius;

	return traverse_true(hurdles, collided_hurdle_generic, arg);
}
