/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of shooting and improvements on moving.
 *
 */

#include "../../application/application.h"
#include "../../input/mouse/mouse.h"
#include "../../input/keyboard/keyboard.h"
#include "../../util/util.h"
#include "../../window/window.h"
#include "../../util/list/list.h"
#include "../../util/math/math.h"
#include "../../util/time/time.h"
#include "../../shape/circle/circle.h"
#include "../../shape/rectangle/rectangle.h"
#include "../hurdle/hurdle.h"
#include "../arena/arena.h"
#include "player.h"
#include <math.h>
#include <GL/glut.h>
#include <stdio.h>


/* Values in [0.0, 1.0] (percentage in function of player speed) */

#define SPIN_RATE 0.6
#define GUN_SPIN_RATE 0.3
#define MAX_GUN_ANGLE 45.0

/***********************************/


struct Player_t
{
	float cx;
	float cy;
	float radius;
	float red;
	float green;
	float blue;
	float angle;

	/* walk stuff */
	float speed;
	int walk_time_elapsed;

	/* shoot stuff */
	int block_shoot;     /* bool */
	float gun_width;
	float gun_height;
	float gun_x;
	float gun_orientation;
	float gun_tip_x;
	float gun_tip_y;

	/* foot stuff */
	float foot_width;
	float foot_height;
	float foot_length;
	float ds_swap_foot;

	/* jumping stuff */
	int jumping;         /* bool */
	int jump_time_elapsed;
	float scale_rate;
};

Player* new_player(float radius, float cx, float cy, float red, float green, float blue)
{
	Player* new;
	get_memory(new, sizeof(Player));
	new->cx = cx;
	new->cy = cy;
	new->red = red;
	new->green = green;
	new->blue = blue;
	new->jumping = 0;
	new->radius = radius;
	/* new->speed = ;  Must be called the set_speed() function before usage. */
	new->foot_width = radius * 0.3;
	new->foot_height = radius * 0.8;
	new->foot_length = 0.0;
	new->gun_width = radius * 0.2;
	new->gun_height = radius * 0.8;
	new->ds_swap_foot = 0.0;
	new->angle = 0.0;
	new->scale_rate = 1.0;
	new->gun_x = radius - new->gun_width;
	new->gun_orientation = 0.0;
	new->gun_tip_x = new->cx + new->gun_x;
	new->block_shoot = 0;
	return new;
}

void delete_player(void** player)
{
	free_memory(&cast_twice(player, Player));
}

void set_speed(Player* player, float speed) { player->speed = speed; }

void adjust_player(Player* player, Window* window)
{
	player->cy = adjust_y(window, player->cy);
	player->gun_tip_y = player->cy + player->gun_height;
}

void display_player(void* player) 
{
	Player* p = player;

	glPushMatrix();

		glTranslatef(p->cx, p->cy, 0.0);
		glScalef(p->scale_rate, p->scale_rate, 1.0);
		glRotatef(p->angle, 0.0, 0.0, 1.0);

		glPushMatrix();
			glTranslatef(0.0, p->foot_width, 0.0);
			display_horizontal_rectangle(p->foot_length, p->foot_width, MIN_COLOR, MIN_COLOR, MIN_COLOR);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(-p->foot_length, -p->foot_width, 0.0);
			display_horizontal_rectangle(p->foot_length, p->foot_width, MIN_COLOR, MIN_COLOR, MIN_COLOR);
		glPopMatrix();


		glPushMatrix();
			glTranslatef(0.0, -p->gun_x, 0.0);
			glRotatef(p->gun_orientation, 0.0, 0.0, 1.0);
			display_horizontal_rectangle(p->gun_height, p->gun_width, p->red, p->green, p->blue);
		glPopMatrix();


		glPushMatrix();
			glScalef(0.15, 1.0, 1.0);
			display_circle(p->radius, p->red, p->green, p->blue);
		glPopMatrix();

		glScalef(0.4, 0.5, 1.0);
		display_circle(p->radius, p->red, p->green, p->blue);

    glPopMatrix();
}

void jump_player(Player* player, Keyboard* keyboard)
{
    if(is_key_pressed(keyboard, 'p') && !player->jumping)
    {
		player->jumping = 1;
		player->jump_time_elapsed = glutGet(GLUT_ELAPSED_TIME);
	}
}

int player_shooted(Player* player, Mouse* mouse)
{
	if(!player->block_shoot && is_left_mouse_pressed(mouse))
	{
		app_add_bullet(player->gun_tip_x, player->gun_tip_y, player->gun_orientation + player->angle);
		return 1;
	}

	return 0;
}

static void update_gun_tip(Player* player) 
{
	player->gun_tip_x = player->cx + sin_rad(player->angle) * player->gun_x + cos_rad(player->angle + player->gun_orientation) * player->gun_height;
	player->gun_tip_y = player->cy - cos_rad(player->angle) * player->gun_x + sin_rad(player->angle + player->gun_orientation) * player->gun_height;
}

int move_gun_player(Player* player, Mouse* mouse)
{
	int dx = get_dx(mouse);

	if(dx)
	{
		player->gun_orientation += dx * GUN_SPIN_RATE;

		if(player->gun_orientation > MAX_GUN_ANGLE) 
			player->gun_orientation = MAX_GUN_ANGLE;
		else if(player->gun_orientation < -MAX_GUN_ANGLE)
			player->gun_orientation = -MAX_GUN_ANGLE;

		update_gun_tip(player);
		
		return 1;
	}

	return 0;
}

static int collided_player(Arena* arena, List* small_hurdles, List* big_hurdles, float cx, float cy, float radius)
{
	return collided_arena(arena, cx, cy, radius)               ||
	       collided_hurdle_list(big_hurdles, cx, cy, radius)   ||
	       collided_hurdle_list(small_hurdles, cx, cy, radius);
}

static void test_swap_foot(Player* player, float ds)
{
	if(!player->jumping)
	{
		player->ds_swap_foot += ds;
		player->foot_length = ((2.0 * player->foot_height) / PI) * asin(sin((TWO_PI / (4.0 * player->foot_height)) * player->ds_swap_foot));
	}
}

int idle_player(Player* player, Arena* arena, List* small_hurdles, List* big_hurdles, Mouse* mouse, Keyboard* keyboard)
{
	int dt;
	int dirty = 0;
	float ds = delta_time(&player->walk_time_elapsed) * player->speed;
	float x_component = cos_rad(player->angle) * ds;
	float y_component = sin_rad(player->angle) * ds;
	unused(mouse);

	player->block_shoot = 0;

	if(player->jumping)
	{
		dirty = 1;
		small_hurdles = NULL;
		player->block_shoot = 1;
		dt = glutGet(GLUT_ELAPSED_TIME) - player->jump_time_elapsed;

		if(dt <= 1000)
		{
			player->scale_rate = (1.0 + dt / 2000.0);
		}
		else if(dt <= 2000)
		{
			player->scale_rate = (1.5 - ((dt - 1000) / 2000.0));
		}
		else
		{
			player->scale_rate = 1.0;
			player->jumping = 0;
		}
	}
	else if(collided_hurdle_list(small_hurdles, player->cx, player->cy, player->radius))  // se esta em cima de um small_hurdle
	{
		small_hurdles = NULL;
		player->block_shoot = 1;
	}

	if(is_key_pressed(keyboard, 'w') && !collided_player(arena, small_hurdles, big_hurdles, player->cx + x_component, player->cy + y_component, player->radius))
	{
		test_swap_foot(player, ds);
        player->cx += x_component;
		player->cy += y_component;
		dirty = 1;
	}

	if(is_key_pressed(keyboard, 's') && !collided_player(arena, small_hurdles, big_hurdles, player->cx - x_component, player->cy - y_component, player->radius))
	{
		test_swap_foot(player, -ds);
        player->cx -= x_component;
		player->cy -= y_component;
		dirty = 1;
	}

    if(is_key_pressed(keyboard, 'd'))
	{
        player->angle -= ds * SPIN_RATE; 
		dirty = 1;
	}

    if(is_key_pressed(keyboard, 'a')) 
	{
        player->angle += ds * SPIN_RATE;
		dirty = 1;
	}

	if(dirty)
		update_gun_tip(player);

   	return dirty;
}
