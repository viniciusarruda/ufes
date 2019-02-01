/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#ifndef CHARACTER_H
#define CHARACTER_H

typedef struct Character_t Character;
typedef enum {PLAYER, ENEMY} Type;

#include "../../window/window.h"
#include "../arena/arena.h"
#include "../hurdles/hurdles.h"
#include "../enemies/enemies.h"
#include "../../scoreboard/scoreboard.h"

Character* new_character(Type type, int id, float radius, float cx, float cy, float red, float green, float blue);

void delete_character(void** character);

int bullet_collided_with_character_void(void* character, void* arg);
int bullet_collided_with_character(Character* character, Type who_shot_type, int who_shot_id, float cx, float cy, float radius, Scoreboard* scoreboard);

void set_speed(Character* character, float speed, float bullet_speed);

void adjust_character_void(void* character, void* window);
void adjust_character(Character* character, Window* window);

void display_character(void* character);

int character_shooted(Character* character, int shoot);

int character_move_gun(Character* character, int dx);

int collided_with_character_void(void* character, void* arg);
int collided_with_character(Character* character, float cx, float cy, float radius);

int get_character_id(Character* character);

void character_jump(Character* character, int jump);

int idle_character(Character* character, Arena* arena, Hurdles* hurdles, Enemies* enemies, Character* player, int up_down, int left_right, int (* collided_extra) (void *, float, float, float), void* arg);

int not_me(void* character, void* id_void);
int is_dead(void* character);


void detects_objects(Character* c, Arena* arena, Hurdles* hurdles, Enemies* enemies, Character* player);
void get_color_by_code(int code, int is_wall_in_front, float* r, float* g, float* b);

int get_sensor_0(Character* character);
int get_sensor_1(Character* character);
int get_sensor_2(Character* character);

int get_consume_movement(Character* character);
void set_consume_movement(Character* character, int consume_movement);
int get_swap_movement(Character* character);
void swap_movement(Character* character);
int get_rotate_state(Character* character);
void toggle_rotate_state(Character* character);

int in_center_space(Character* character);
void set_center_space(Character* character);
int get_dt_space(Character* character);
void reset_dt_space(Character* character);

void reset_dt_shoot(Character* character);
int get_dt_shoot(Character* character);

#endif
