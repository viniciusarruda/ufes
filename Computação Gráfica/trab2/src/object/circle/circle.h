/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a player and hurdles with collision detection and jump effect.
 *
 */
 
#ifndef CIRCLE_H
#define CIRCLE_H

void display_circle(float radius, float cx, float cy, float red, float green, float blue);
int collided_circle(float cx1, float cy1, float radius1, float cx2, float cy2, float radius2);
int is_inside(float cx1, float cy1, float radius1, float cx2, float cy2, float radius2);

#endif
