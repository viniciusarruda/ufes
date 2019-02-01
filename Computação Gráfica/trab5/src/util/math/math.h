/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#ifndef NON_CONFLICT_MATH_H
#define NON_CONFLICT_MATH_H

#include <math.h>

/* MATH CONSTANTS */
#define PI     3.14159265359
#define TWO_PI 6.28318530718

#define pythagorean_c(a, b) (sqrtf(((a) * (a)) + ((b) * (b))))
#define to_rad(angle) ((PI * (angle)) / 180.0)
#define to_angle(rad) ((rad * 180.0) / PI)

#define cos_rad(a) (cos(to_rad((a))))
#define sin_rad(a) (sin(to_rad((a))))


int line_intercepts_circle(float cx, float cy, float radius, float x0, float y0, float x1, float y1, float* rx, float* ry);
int point_inside_circle(float cx, float cy, float r, float x, float y);

#endif
