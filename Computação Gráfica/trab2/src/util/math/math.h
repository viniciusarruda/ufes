/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a player and hurdles with collision detection and jump effect.
 *
 */
 
#ifndef NON_CONFLICT_MATH_H
#define NON_CONFLICT_MATH_H

#include <math.h>

/* MATH CONSTANTS */
#define PI     3.14159265359
#define TWO_PI 6.28318530718

#define pythagorean_c(a, b) (sqrt(((a) * (a)) + ((b) * (b))))

void project_s_onto_l(int sx, int sy, int lx, int ly, double* project_x, double* project_y);

#endif
