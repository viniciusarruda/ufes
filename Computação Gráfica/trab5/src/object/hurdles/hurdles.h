/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#ifndef HURDLES_H
#define HURDLES_H

#include "../../window/window.h"

typedef struct Hurdles_t Hurdles;

Hurdles* new_hurdles(void);
void set_percentage(Hurdles* hurdles, float percentage);
void add_hurdle(Hurdles* hurdles, float radius, float cx, float cy, float red, float green, float blue);
void delete_hurdles(Hurdles** hurdles);
void display_hurdles(Hurdles* hurdles);
void display_mini_hurdles(Hurdles* hurdles);

int collided_hurdles(Hurdles* hurdles, float cx, float cy, float radius);
int collided_bullet_hurdles(Hurdles* hurdles, float cx, float cy, float cz);

void adjust_hurdles(Hurdles* hurdles, Window* window, float height);
float hurdles_scale_high(Hurdles* hurdles);

float get_hurdle_cx(void* hurdle);
float get_hurdle_cy(void* hurdle);
float get_hurdle_r(void* hurdle);

void apply_for_all_hurdles(Hurdles* hurdles, void (* apply) (void *, void *), void* arg);

#endif
