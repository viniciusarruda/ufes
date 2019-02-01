/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#ifndef CIRCLE_H
#define CIRCLE_H

void display_circle(float radius, float red, float green, float blue);
void display_disk(float radius, float red, float green, float blue);
int collided_circle(float cx1, float cy1, float radius1, float cx2, float cy2, float radius2);
int is_inside(float cx1, float cy1, float radius1, float cx2, float cy2, float radius2);
//int collided_sphere(float cx1, float cy1, float cz1, float radius1, float cx2, float cy2, float cz2, float radius2);
int point_collided_cylinder(float px, float py, float pz, float cx, float cy, float base_z, float top_z, float cr);
#endif
