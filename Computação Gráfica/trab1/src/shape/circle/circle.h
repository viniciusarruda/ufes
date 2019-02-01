/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */
 
#ifndef CIRCLE_H
#define CIRCLE_H

#include "../../color/color.h"
#include "../shape.h"

typedef struct
{
	/* private */
	double radius;
	double min_radius;
	int x; /* center x */
	int y; /* center y */
	Color* color;

	/* public */
	Display* display; //void (* display) (void *);
} Circle;

Circle* new_circle(double radius, double min_radius, int x, int y, Color* color);
void delete_circle(Circle** circle);
void deep_delete_circle(Circle** circle);
int point_in_circle(Circle* circle, int x, int y);

double get_radius(Circle* circle);
void set_radius(Circle* circle, double radius);

int get_center_x(Circle* circle);
int get_center_y(Circle* circle);

void set_center_x(Circle* circle, int x);
void set_center_y(Circle* circle, int y);

#endif
