/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#include <GL/gl.h>

void display_vertical_rectangle(float width, float height, float red, float green, float blue)
{
	glColor3f(red, green, blue);
	glRectf(- width * 0.5, 0.0, width * 0.5, height);
}

void display_horizontal_rectangle(float width, float height, float red, float green, float blue)
{
	glColor3f(red, green, blue);
	glRectf(0.0, height * 0.5, width, - height * 0.5);
}

void display_centered_rectangle(float width, float height, float red, float green, float blue)
{
	width *= 0.5;
	height *= 0.5;
	glColor3f(red, green, blue);
	glRectf(-width, -height, width, height);
}

void display_corner_L_rectangle(float width, float height, float red, float green, float blue)
{
	glColor3f(red, green, blue);
	glRectf(0, 0, width, height);
}
