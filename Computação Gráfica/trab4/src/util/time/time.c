/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */

#include <GL/glut.h>

int delta_time(int* time_last_update)
{
	int now = glutGet(GLUT_ELAPSED_TIME);
	int delta = now - *time_last_update;
	*time_last_update = now;
	return delta;
}


