/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a player and hurdles with collision detection and jump effect.
 *
 */

#include "../../input/mouse/mouse.h"
#include "../../input/keyboard/keyboard.h"
#include "../../util/util.h"
#include "../../window/window.h"
#include "../../util/list/list.h"
#include "../circle/circle.h"
#include "../hurdle/hurdle.h"
#include "../arena/arena.h"
#include "player.h"
#include <stdio.h>
#include <GL/glut.h>

#define SPEED 1

struct Player_t
{
	float radius;
	float cx;
	float cy;
	float red;
	float green;
	float blue;

	/* jumping stuff */
	float original_radius;
	int jumping;
	int timeelapsed;
};

void adjust_player(Player* player, Window* window)
{
	player->cy = adjust_y(window, player->cy);
}

void delete_player(void** player)
{
	free_memory(&cast_twice(player, Player));
}

void display_player(void* player) 
{
	display_circle(cast(player, Player)->radius, cast(player, Player)->cx, cast(player, Player)->cy, cast(player, Player)->red, cast(player, Player)->green, cast(player, Player)->blue);
}

Player* new_player(float radius, float cx, float cy, float red, float green, float blue)
{
	Player* new;
	get_memory(new, sizeof(Player));
	new->radius = radius;
	new->cx = cx;
	new->cy = cy;
	new->red = red;
	new->green = green;
	new->blue = blue;
	new->jumping = 0;
	new->original_radius = radius;
	return new;
}

void jump_player(Player* player, Keyboard* keyboard)
{
    if(is_key_pressed(keyboard, 'p') && !player->jumping)
    {
		player->jumping = 1;
		player->timeelapsed = glutGet(GLUT_ELAPSED_TIME);
	}
}

// testar com o preto colado no vermelho

static int collided_player(Arena* arena, List* small_hurdles, List* big_hurdles, float cx, float cy, float radius)
{
	return collided_arena(arena, cx, cy, radius)               ||
	       collided_hurdle_list(big_hurdles, cx, cy, radius)   ||
	       (small_hurdles != NULL && collided_hurdle_list(small_hurdles, cx, cy, radius));
}

int idle_player(Player* player, Arena* arena, List* small_hurdles, List* big_hurdles, Mouse* mouse, Keyboard* keyboard)
{
	int dt;
	int dirty = 0;
	unused(mouse);

	if(player->jumping)
	{
		small_hurdles = NULL;
		dirty = 1;
		dt = glutGet(GLUT_ELAPSED_TIME) - player->timeelapsed;

		if(dt <= 1000)
		{
			player->radius = player->original_radius * (1.0 + dt / 2000.0);
		}
		else if(dt <= 2000)
		{
			player->radius = player->original_radius * (1.5 - ((dt - 1000) / 2000.0));
		}
		else
		{
			player->radius = player->original_radius;
			player->jumping = 0;
		}
	}
	else if(collided_hurdle_list(small_hurdles, player->cx, player->cy, player->original_radius))  // se esta em cima de um small_hurdle
	{
		small_hurdles = NULL;
	}


	if(is_key_pressed(keyboard, 'w') && !collided_player(arena, small_hurdles, big_hurdles, player->cx, player->cy + SPEED, player->original_radius))
	{
        player->cy += SPEED;
		dirty = 1;
	}

	if(is_key_pressed(keyboard, 's') && !collided_player(arena, small_hurdles, big_hurdles, player->cx, player->cy - SPEED, player->original_radius))
	{
        player->cy -= SPEED;
		dirty = 1;
	}

    if(is_key_pressed(keyboard, 'd') && !collided_player(arena, small_hurdles, big_hurdles, player->cx + SPEED, player->cy, player->original_radius))
	{
        player->cx += SPEED;
		dirty = 1;
	}

    if(is_key_pressed(keyboard, 'a') && !collided_player(arena, small_hurdles, big_hurdles, player->cx - SPEED, player->cy, player->original_radius))
	{
        player->cx -= SPEED;
		dirty = 1;
	}

   	return dirty;
}
