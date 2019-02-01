/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#include "../../util/util.h"
#include "../../window/window.h"
#include "../../util/list/list.h"
#include "../arena/arena.h"
#include "ai/ai.h"
#include "../../util/bmp_loader/bmp_loader.h"
#include "../hurdles/hurdles.h"
#include "../character/character.h"
#include "../../scoreboard/scoreboard.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "enemies.h"

struct Enemies_t
{
	float freq_shot;
	List* enemies;
};

Enemies* new_enemies(void)
{
	Enemies* new;
	get_memory(new, sizeof(Enemies));
	new->enemies = new_list();
	return new;
}

void set_shot_freq(Enemies* enemies, float freq_shot)
{
	enemies->freq_shot = freq_shot;
}

int number_of_enemies(Enemies* enemies)
{
	return get_size(enemies->enemies);
}

void add_enemy(Enemies* enemies, int id, float radius, float cx, float cy, float red, float green, float blue)
{
	append(enemies->enemies, new_character(ENEMY, id, radius, cx, cy, red, green, blue)); 
}

void adjust_enemies(Enemies* enemies, Window* window)
{
	GLuint texture = LoadTexture("texture/enemy.bmp");
	traverse_arg(enemies->enemies, adjust_character_void, window);
	traverse_arg(enemies->enemies, set_texture, &texture);
}

void delete_enemies(Enemies** enemies)
{
	delete_list(&(*enemies)->enemies, delete_character);
	free_memory(enemies);
}

void display_mini_enemies(Enemies* enemies)
{
	traverse(enemies->enemies, display_mini_character);
}

void display_enemies(Enemies* enemies)
{
	traverse(enemies->enemies, display_character);
}

static void set_each_enemy_speed(void* enemy, void* arg)
{
	float* args = arg;
	set_speed(enemy, args[0], args[1]);
}

void set_enemies_speed(Enemies* enemies, float speed, float bullet_speed)
{
	float arg[] = {speed, bullet_speed};
	traverse_arg(enemies->enemies, set_each_enemy_speed, arg);
} 

int collided_with_enemies(Enemies* enemies, float cx, float cy, float radius)
{
	float arg[] = {cx, cy, radius};
	return enemies != NULL && traverse_true(enemies->enemies, collided_with_character_void, arg);
}

int bullet_collided_with_enemies(Enemies* enemies, Type who_shot_type, int who_shot_id, float cx, float cy, float cz, Scoreboard* scoreboard)
{
	void* arg[] = {&cx, &cy, &cz, &who_shot_id, &who_shot_type, scoreboard};
	int ret = enemies != NULL && traverse_true(enemies->enemies, bullet_collided_with_character_void, arg);
	traverse_conditional_delete_one(enemies->enemies, is_dead, delete_character);
	return ret;
}

int enemy_collided_with_enemies(Enemies* enemies, int id, float cx, float cy, float radius)
{
	float arg[] = {cx, cy, radius};
	return enemies != NULL && traverse_true_conditional(enemies->enemies, not_me, &id, collided_with_character_void, arg);
}

static int enemy_collided(void* arg, float cx, float cy, float radius)
{
	void** arg_void = (void **) arg;

	return collided_with_character(cast(arg_void[1], Character), cx, cy, radius) ||
	       enemy_collided_with_enemies(cast(arg_void[0], Enemies), *cast(arg_void[2], int), cx, cy, radius);
}

static int enemy_behave_tmp(void* enemy, void* arg_void) 
{
	void** args = (void **) arg_void;
	int id = get_character_id(enemy);
	void* argss[] = {args[2], args[3], &id};
	int up_down, left_right, jump, shoot;

	ai_output(enemy, (((Enemies **)args)[2])->freq_shot, &up_down, &left_right, &jump, &shoot);

	character_jump(enemy, jump);

	return character_shooted(enemy, shoot)        ||
		   idle_character(enemy, args[0], args[1], args[2], args[3], up_down, left_right, enemy_collided, argss);
}

int enemies_behave(Enemies* enemies, Character* player, Arena* arena, Hurdles* hurdles)
{
	void* arg[] = {arena, hurdles, enemies, player};
	return traverse_any_all(enemies->enemies, enemy_behave_tmp, arg);
}


void apply_for_all_enemies(Enemies* enemies, void (* apply) (void *, void *), void* arg)
{
	traverse_arg(enemies->enemies, apply, arg);
}


