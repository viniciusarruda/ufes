/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */
 
#include "../util/get_memory.h"
#include "../util/error.h"
#include "color.h"
#include <stdlib.h>

#define MIN_COLOR 0.0
#define MAX_COLOR 1.0

struct Color_t
{
	double r;
	double g;
	double b;
};

static void assert_boundary(double c)
{
	if(c < MIN_COLOR || c > MAX_COLOR)
		raise_error("The specified color is out of correctly range.");
}

Color* new_color(double r, double g, double b)
{
	Color* new;
	get_memory(new, sizeof(Color));
	assert_boundary(r);
	assert_boundary(g);
	assert_boundary(b);
	new->r = r;
	new->g = g;
	new->b = b;
	return new;
}

void delete_color(Color** color)
{
	free(*color);
	color = NULL;
}

void set_red(Color* color, double r)
{
	assert_boundary(r);
	color->r = r;
}

void set_green(Color* color, double g)
{
	assert_boundary(g);
	color->g = g;
}

void set_blue(Color* color, double b)
{
	assert_boundary(b);
	color->b = b;
}

double get_red(Color* color) { return color->r; }
double get_green(Color* color) { return color->g; }
double get_blue(Color* color) { return color->b; }


