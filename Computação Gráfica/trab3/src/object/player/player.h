/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of shooting and improvements on moving.
 *
 */
 
#ifndef PLAYER_H
#define PLAYER_H

#include "../../input/mouse/mouse.h"
#include "../../input/keyboard/keyboard.h"
#include "../../window/window.h"
#include "../../util/list/list.h"
#include "../arena/arena.h"

typedef struct Player_t Player;

Player* new_player(float radius, float cx, float cy, float red, float green, float blue);
int idle_player(Player* player, Arena* arena, List* small_hurdles, List* big_hurdles, Mouse* mouse, Keyboard* keyboard);
void jump_player(Player* player, Keyboard* keyboard);
void delete_player(void** player);
void display_player(void* player);
void adjust_player(Player* player, Window* window);
void set_speed(Player* player, float speed);
int move_gun_player(Player* player, Mouse* mouse);
int player_shooted(Player* player, Mouse* mouse);

#endif
