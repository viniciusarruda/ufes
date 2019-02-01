/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#ifndef ENEMIES_H
#define ENEMIES_H

typedef struct Enemies_t Enemies;

#include "../../window/window.h"
#include "../arena/arena.h"
#include "../hurdles/hurdles.h"
#include "../character/character.h"
#include "../../scoreboard/scoreboard.h"

Enemies* new_enemies(void);
void add_enemy(Enemies* enemies, int id, float radius, float cx, float cy, float red, float green, float blue);
void delete_enemies(Enemies** enemies);
void display_enemies(Enemies* enemies);
void display_mini_enemies(Enemies* enemies);
int collided_with_enemies(Enemies* enemies, float cx, float cy, float radius);
void adjust_enemies(Enemies* enemies, Window* window);
int enemies_behave(Enemies* enemies, Character* player, Arena* arena, Hurdles* hurdles);
void set_enemies_speed(Enemies* enemies, float speed, float bullet_speed);
int enemy_collided_with_enemies(Enemies* enemies, int id, float cx, float cy, float radius);
int bullet_collided_with_enemies(Enemies* enemies, Type who_shot_type, int who_shot_id, float cx, float cy, float cz, Scoreboard* scoreboard);
int number_of_enemies(Enemies* enemies);
void apply_for_all_enemies(Enemies* enemies, void (* apply) (void *, void *), void* arg);
void set_shot_freq(Enemies* enemies, float freq_shot);

#endif
