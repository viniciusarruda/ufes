/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of shooting and improvements on moving.
 *
 */
 
#ifndef BULLETS_H
#define BULLETS_H

#include "../arena/arena.h"
#include "../../util/list/list.h"

typedef struct Bullets_t Bullets;

Bullets* new_bullets(float bullet_speed);
void delete_bullets(Bullets** bullets);
void add_bullet(Bullets* bullets, float cx, float cy, float orientation);
void display_bullets(Bullets* bullets, Arena* arena, List* small_hurdles, List* big_hurdles);
int has_bullets(Bullets* bullets);

#endif
