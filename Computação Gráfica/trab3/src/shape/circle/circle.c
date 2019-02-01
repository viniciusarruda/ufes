/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of shooting and improvements on moving.
 *
 */
 
#include "../../util/math/math.h"
#include <GL/gl.h>

#define N_TRIANGLES 100
#define OFFSET ((TWO_PI) / ((float) N_TRIANGLES)) /* I think gcc is clever enought to make this calc just once... */

void display_circle(float radius, float red, float green, float blue)
{
	int i;
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0.0, 0.0);
		for(i = 0; i <= N_TRIANGLES; i++) 
			glVertex2f(radius * cos(i *  OFFSET), radius * sin(i * OFFSET));
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
