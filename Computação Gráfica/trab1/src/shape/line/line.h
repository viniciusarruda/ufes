/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */
 
#ifndef LINE_H
#define LINE_H

#include <math.h>

#define pythagorean_c(a, b) (sqrt(((a) * (a)) + ((b) * (b))))

void project_s_onto_l(int sx, int sy, int lx, int ly, double* project_x, double* project_y);

#endif
