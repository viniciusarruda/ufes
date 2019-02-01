/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#include "../../util/math/math.h"
#include "../../util/global/global.h"
#include <GL/gl.h>
#include <GL/glut.h>

#define N_TRIANGLES 100
#define OFFSET ((TWO_PI) / ((float) N_TRIANGLES)) /* I think gcc is clever enought to make this calc just once... */


void display_circle(float radius, float red, float green, float blue)
{
	static float x[N_TRIANGLES + 1];
	static float y[N_TRIANGLES + 1];
	static int runned = 0;
	int i;

	if(runned)
	{
		glPushMatrix();
			glScalef(radius, radius, 1.0);
			glColor3f(red, green, blue);
			glBegin(GL_LINE_LOOP);
				for(i = 0; i <= N_TRIANGLES; i++) 
					glVertex2f(x[i], y[i]);
			glEnd();
		glPopMatrix();
	}
	else
	{
		runned = 1;
		for(i = 0; i <= N_TRIANGLES; i++) 
		{
			x[i] = cos(i * OFFSET);
			y[i] = sin(i * OFFSET);
		}
		display_circle(radius, red, green, blue);
	}
}


// void display_circle(float radius, float red, float green, float blue)
// {
// 	int i;
// 	glColor3f(red, green, blue);
// 	glBegin(GL_LINE_LOOP);
// 		for(i = 0; i <= N_TRIANGLES; i++) 
// 			glVertex2f(radius * cos(i *  OFFSET), radius * sin(i * OFFSET));
// 	glEnd();
// }

void display_disk(float radius, float red, float green, float blue)
{
	glColor3f(red, green, blue);
	GLUquadric* quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);
	gluDisk(quad, 0, radius, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
	gluDeleteQuadric(quad);
}

int collided_circle(float cx1, float cy1, float radius1, float cx2, float cy2, float radius2)
{
	return (cx2 - cx1) * (cx2 - cx1) + (cy1 - cy2) * (cy1 - cy2) <= (radius1 + radius2) * (radius1 + radius2);
}

int is_inside(float cx1, float cy1, float radius1, float cx2, float cy2, float radius2)
{
	return (cx1 - cx2) * (cx1 - cx2) + (cy2 - cy1) * (cy2 - cy1) <= (radius2 - radius1) * (radius2 - radius1);
}

// int collided_sphere(float cx1, float cy1, float cz1, float radius1, float cx2, float cy2, float cz2, float radius2)
// {
// 	return (cx2 - cx1) * (cx2 - cx1) + (cy1 - cy2) * (cy1 - cy2) + (cz1 - cz2) * (cz1 - cz2) <= (radius1 + radius2) * (radius1 + radius2);
// }


int point_collided_cylinder(float px, float py, float pz, float cx, float cy, float base_z, float top_z, float cr)
{
	return (cx - px) * (cx - px) + (cy - py) * (cy - py) <= cr * cr &&
	       base_z <= pz && pz <= top_z; 
}
