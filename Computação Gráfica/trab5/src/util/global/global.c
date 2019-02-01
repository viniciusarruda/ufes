/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */

#include "global.h"
#include <GL/glut.h>

GLfloat* global_mat(Mat_type type)
{
	static GLfloat global_none[] = {0.0, 0.0, 0.0, 1.0};
	static GLfloat global_shininess[] = {128};
	static GLfloat global_hard_low[] = { 0.1, 0.1, 0.1, 1.0};
	static GLfloat global_low[] = { 0.2, 0.2, 0.2, 1.0};
	static GLfloat global_high[] = { 0.8, 0.8, 0.8, 1.0};
	static GLfloat global_full[] = { 1.0, 1.0, 1.0, 1.0};

	switch(type)
	{
		case NONE_mat: return global_none; break;
		case SHININESS_mat: return global_shininess; break;
		case LOW_mat: return global_low; break;
		case HARD_LOW_mat: return global_hard_low; break;
		case HIGH_mat: return global_high; break;
		case FULL_mat: return global_full; break;
	}

	return NULL;
}
