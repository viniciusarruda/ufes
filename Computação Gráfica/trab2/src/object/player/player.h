/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a player and hurdles with collision detection and jump effect.
 *
 */
 
#ifndef PLAYER_H
#define PLAYER_H

#include "../../input/mouse/mouse.h"
#include "../../input/keyboard/keyboard.h"
#include "../../window/window.h"
#include "../../util/list/list.h"

typedef struct Player_t Player;

Player* new_player(float radius, float cx, float cy, float red, float green, float blue);
int idle_player(Player* player, Arena* arena, List* small_hurdles, List* big_hurdles, Mouse* mouse, Keyboard* keyboard);
void jump_player(Player* player, Keyboard* keyboard);
void delete_player(void** player);
void display_player(void* player);
void adjust_player(Player* player, Window* window);

#endif
