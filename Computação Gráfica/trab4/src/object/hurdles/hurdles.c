/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#include "../../util/util.h"
#include "../../window/window.h"
#include "../../util/list/list.h"
#include "../../shape/circle/circle.h"
#include <GL/glut.h>
#include "hurdles.h"

typedef struct
{
	float radius;
	float cx;
	float cy;
	float red;
	float green;
	float blue;
} Hurdle;

struct Hurdles_t
{
	float scale_high;
	List* hurdles;
};

static Hurdle* new_hurdle(float radius, float cx, float cy, float red, float green, float blue)
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

Hurdles* new_hurdles(void)
{
	Hurdles* new;
	get_memory(new, sizeof(Hurdles));
	new->hurdles = new_list();
	return new;
}

void set_percentage(Hurdles* hurdles, float percentage)
{
	hurdles->scale_high = 1.0 + 0.005 * percentage;
}

float hurdles_scale_high(Hurdles* hurdles)
{
	return hurdles->scale_high;
}

void add_hurdle(Hurdles* hurdles, float radius, float cx, float cy, float red, float green, float blue)
{
	append(hurdles->hurdles, new_hurdle(radius, cx, cy, red, green, blue)); 
}

static void adjust_hurdle(void* hurdle, void* window)
{
	cast(hurdle, Hurdle)->cy = adjust_y(window, cast(hurdle, Hurdle)->cy);
}

void adjust_hurdles(Hurdles* hurdles, Window* window)
{
	traverse_arg(hurdles->hurdles, adjust_hurdle, window);
}

static void delete_hurdle(void** hurdle)  
{
	free_memory(&cast_twice(hurdle, Hurdle));
}

void delete_hurdles(Hurdles** hurdles)
{
	delete_list(&(*hurdles)->hurdles, delete_hurdle);
	free_memory(hurdles);
}

static void display_hurdle(void* hurdle)
{
	glPushMatrix();
		
		glTranslatef(cast(hurdle, Hurdle)->cx, cast(hurdle, Hurdle)->cy, 0.0);
		display_circle(cast(hurdle, Hurdle)->radius, cast(hurdle, Hurdle)->red, cast(hurdle, Hurdle)->green, cast(hurdle, Hurdle)->blue);

	glPopMatrix();		
}

void display_hurdles(Hurdles* hurdles)
{
	traverse(hurdles->hurdles, display_hurdle);
}

static int collided_hurdle_generic(void* hurdle, void* arg)
{
	float* arg_array = arg;

	return collided_circle(cast(hurdle, Hurdle)->cx, cast(hurdle, Hurdle)->cy, cast(hurdle, Hurdle)->radius, arg_array[0], arg_array[1], arg_array[2]);
}

int collided_hurdles(Hurdles* hurdles, float cx, float cy, float radius)
{
	float arg[3]; 

	arg[0] = cx;
	arg[1] = cy;
	arg[2] = radius;

	return hurdles != NULL && traverse_true(hurdles->hurdles, collided_hurdle_generic, arg);
}


float get_hurdle_cx(void* hurdle)
{
	return cast(hurdle, Hurdle)->cx;
}

float get_hurdle_cy(void* hurdle)
{
	return cast(hurdle, Hurdle)->cy;
}

float get_hurdle_r(void* hurdle)
{
	return cast(hurdle, Hurdle)->radius;
}

void apply_for_all_hurdles(Hurdles* hurdles, void (* apply) (void *, void *), void* arg)
{
	traverse_arg(hurdles->hurdles, apply, arg);
}



