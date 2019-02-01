/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of shooting and improvements on moving.
 *
 */
 
#ifndef NON_CONFLICT_MATH_H
#define NON_CONFLICT_MATH_H

#include <math.h>

/* MATH CONSTANTS */
#define PI     3.14159265359
#define TWO_PI 6.28318530718

#define pythagorean_c(a, b) (sqrt(((a) * (a)) + ((b) * (b))))
#define to_rad(angle) ((PI * (angle)) / 180.0)

#define cos_rad(a) (cos(to_rad((a))))
#define sin_rad(a) (sin(to_rad((a))))

#endif
