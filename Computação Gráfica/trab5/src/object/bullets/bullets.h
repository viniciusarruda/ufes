/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#ifndef BULLETS_H
#define BULLETS_H

#include "../arena/arena.h"
#include "../enemies/enemies.h"
#include "../hurdles/hurdles.h"
#include "../character/character.h"

typedef struct Bullets_t Bullets;

Bullets* new_bullets(void);
void delete_bullets(Bullets** bullets);
void add_bullet(Bullets* bullets, Type who_shot_type, int who_shot_id, float cx, float cy, float cz, float ux, float uy, float uz, float bullet_speed);
int has_bullets(Bullets* bullets);
void display_bullets(Bullets* bullets, Arena* arena, Hurdles* hurdles, Enemies* enemies, Character* player, Scoreboard* scoreboard);

#endif
