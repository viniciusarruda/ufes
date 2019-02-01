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

void display_line(float x0, float y0, float x1, float y1, float red, float green, float blue)
{
	glColor3f(red, green, blue);

	glBegin(GL_LINES);
		glVertex2f(x0, y0);
		glVertex2f(x1, y1);
	glEnd();
}


