/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a player and hurdles with collision detection and jump effect.
 *
 */
 
#include "../../util/math/math.h"
#include <GL/gl.h>

#define N_TRIANGLES 100
#define OFFSET ((TWO_PI) / ((float) N_TRIANGLES)) /* I think gcc is clever enought to make this calc just once... */

void display_circle(float radius, float cx, float cy, float red, float green, float blue)
{
	int i;
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(cx, cy);
		for(i = 0; i <= N_TRIANGLES; i++) 
			glVertex2f(cx + (radius * cos(i *  OFFSET)), cy + (radius * sin(i * OFFSET)));
	glEnd();
}

int collided_circle(float cx1, float cy1, float radius1, float cx2, float cy2, float radius2)
{
	return (cx2 - cx1) * (cx2 - cx1) + (cy1 - cy2) * (cy1 - cy2) <= (radius1 + radius2) * (radius1 + radius2);
}

int is_inside(float cx1, float cy1, float radius1, float cx2, float cy2, float radius2)
{
	return (cx1 - cx2) * (cx1 - cx2) + (cy2 - cy1) * (cy2 - cy1) <= (radius2 - radius1) * (radius2 - radius1);
}
