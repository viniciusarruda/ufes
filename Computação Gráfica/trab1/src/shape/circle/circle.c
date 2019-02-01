/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */
 
#include "../../util/get_memory.h"
#include "../../util/pi.h"
#include "../../util/error.h"
#include "../../color/color.h"
#include "../line/line.h"
#include "../shape.h"
#include "circle.h"
#include <stdlib.h>
#include <GL/gl.h>
#include <stdio.h>

#define CAST(circle) ((Circle *) (circle))

#define N_TRIANGLES 100
#define OFFSET ((TWO_PI) / ((double) N_TRIANGLES)) /* I think gcc is clever enought to make this calc just once... */

static void display_circle(void* circle);

Circle* new_circle(double radius, double min_radius, int x, int y, Color* color)
{
	Circle* new;
	get_memory(new, sizeof(Circle));
	new->radius = radius;
	new->min_radius = min_radius;
	new->x = x;
	new->y = y;
	new->color = color;
	new->display = display_circle;
	return new;
}

void delete_circle(Circle** circle)
{
	free(*circle);
	circle = NULL;
}

void deep_delete_circle(Circle** circle)
{
	delete_color(&(*circle)->color);
	free(*circle);
	circle = NULL;
}

int point_in_circle(Circle* circle, int x, int y)
{
	return pythagorean_c(x - circle->x, y - circle->y) <= circle->radius;
}

static void display_circle(void* circle)
{
	int i;

	glColor3f(get_red(CAST(circle)->color), get_green(CAST(circle)->color), get_blue(CAST(circle)->color));
	
	glBegin(GL_TRIANGLE_FAN);
	
		glVertex2f(CAST(circle)->x, CAST(circle)->y);
	
		for(i = 0; i <= N_TRIANGLES; i++) 
		{ 
			glVertex2f(
						CAST(circle)->x + (CAST(circle)->radius * cos(i *  OFFSET)), 
						CAST(circle)->y + (CAST(circle)->radius * sin(i * OFFSET))
					  );
		}
		
	glEnd();
}


double get_radius(Circle* circle) { return circle->radius; }

void set_radius(Circle* circle, double radius)
{ 
	circle->radius = (radius < circle->min_radius) ? circle->min_radius : radius; 
}

int get_center_x(Circle* circle) { return circle->x; }
int get_center_y(Circle* circle) { return circle->y; }

void set_center_x(Circle* circle, int x) { circle->x = x; }
void set_center_y(Circle* circle, int y) { circle->y = y; }
