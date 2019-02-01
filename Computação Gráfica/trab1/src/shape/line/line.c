/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */
 
#include <math.h>
#include <stdio.h>

void project_s_onto_l(int sx, int sy, int lx, int ly, double* project_x, double* project_y)
{	
	double tmp = (sx * lx + sy * ly) / (double) (lx * lx + ly * ly);
	*project_x = tmp * lx;
	*project_y = tmp * ly;
}

