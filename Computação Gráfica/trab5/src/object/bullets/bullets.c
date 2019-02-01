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
#include "../../util/global/global.h"
#include "../../util/list/list.h"
#include "../../util/math/math.h"  
#include "../../shape/circle/circle.h"
#include "../../util/time/time.h"
#include "../arena/arena.h"
#include "../enemies/enemies.h"
#include "../hurdles/hurdles.h"
#include "../../scoreboard/scoreboard.h"
#include "../character/character.h"
#include <stdlib.h>
#include <GL/glut.h>
#include "bullets.h"

#define BULLET_RADIUS 2.0

typedef struct
{
	Type who_shot_type;
	int who_shot_id; 
	
	float cx;
	float cy;
	float cz;
	float x_dir;
	float y_dir;
	float z_dir;
	
	int time_last_update;
	float bullet_speed;
} Bullet;

struct Bullets_t
{
	List* bullets;
};

int has_bullets(Bullets* bullets)
{
	return get_size(bullets->bullets) != 0;
}

static Bullet* new_bullet(Type who_shot_type, int who_shot_id, float cx, float cy, float cz, float ux, float uy, float uz, float bullet_speed)
{
	Bullet* new;
	get_memory(new, sizeof(Bullet));
	new->who_shot_type = who_shot_type;
	new->who_shot_id = who_shot_id;
	new->bullet_speed = bullet_speed;
	
	new->cx = cx;
	new->cy = cy;
	new->cz = cz;
	new->x_dir = ux;
	new->y_dir = uy;
	new->z_dir = uz;
	
	new->time_last_update = glutGet(GLUT_ELAPSED_TIME);
	return new;
}

Bullets* new_bullets(void)
{
	Bullets* new;
	get_memory(new, sizeof(Bullets));
	new->bullets = new_list();
	return new;
}

static void delete_bullet(void** bullet)  
{
	free_memory(&cast_twice(bullet, Bullet));
}

void delete_bullets(Bullets** bullets)
{
	delete_list(&(*bullets)->bullets, delete_bullet);
	free_memory(bullets);
}

void add_bullet(Bullets* bullets, Type who_shot_type, int who_shot_id, float cx, float cy, float cz, float ux, float uy, float uz, float bullet_speed)
{
	//return; // disabled
	append(bullets->bullets, new_bullet(who_shot_type, who_shot_id, cx, cy, cz, ux, uy, uz, bullet_speed));
}

static void update_bullet(void* bullet)
{
	float ds = delta_time(&cast(bullet, Bullet)->time_last_update) * cast(bullet, Bullet)->bullet_speed;

	cast(bullet, Bullet)->cx += cast(bullet, Bullet)->x_dir * ds;
	cast(bullet, Bullet)->cy += cast(bullet, Bullet)->y_dir * ds;
	cast(bullet, Bullet)->cz += cast(bullet, Bullet)->z_dir * ds;
}

static int bullet_collided(void* bullet, void* arg)  
{
	Arena* arena = ((void **) arg)[0];
	Hurdles* hurdles = ((void **) arg)[1];
	Enemies* enemies = ((void **) arg)[2];
	Character* player = ((void **) arg)[3];
	Scoreboard* scoreboard = ((void **) arg)[4];

	return collided_arena(arena, cast(bullet, Bullet)->cx, cast(bullet, Bullet)->cy, BULLET_RADIUS)        ||
	       collided_bullet_hurdles(hurdles, cast(bullet, Bullet)->cx, cast(bullet, Bullet)->cy, cast(bullet, Bullet)->cz) || 
	       bullet_collided_with_character(player, cast(bullet, Bullet)->who_shot_type, cast(bullet, Bullet)->who_shot_id, cast(bullet, Bullet)->cx, cast(bullet, Bullet)->cy, cast(bullet, Bullet)->cz, scoreboard) ||
	       bullet_collided_with_enemies(enemies, cast(bullet, Bullet)->who_shot_type, cast(bullet, Bullet)->who_shot_id, cast(bullet, Bullet)->cx, cast(bullet, Bullet)->cy, cast(bullet, Bullet)->cz, scoreboard);
}

static void display_bullet(void* bullet)
{
	glPushMatrix();
		glTranslatef(cast(bullet, Bullet)->cx, cast(bullet, Bullet)->cy, cast(bullet, Bullet)->cz);
		glutSolidSphere(BULLET_RADIUS, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
	glPopMatrix();	
}

void display_bullets(Bullets* bullets, Arena* arena, Hurdles* hurdles, Enemies* enemies, Character* player, Scoreboard* scoreboard) 
{	
	void* arg[] = {arena, hurdles, enemies, player, scoreboard};

	glMaterialfv(GL_FRONT, GL_SPECULAR, global_mat(NONE_mat));
    glMaterialfv(GL_FRONT, GL_SHININESS, global_mat(SHININESS_mat));
    glMaterialfv(GL_FRONT, GL_EMISSION, global_mat(NONE_mat));
    glMaterialfv(GL_FRONT, GL_AMBIENT, global_mat(HARD_LOW_mat));  
    glMaterialfv(GL_FRONT, GL_DIFFUSE, global_mat(HARD_LOW_mat));

	traverse(bullets->bullets, update_bullet);              /* update bullet position      */
	traverse_conditional_delete(bullets->bullets, bullet_collided, arg, delete_bullet); /* delete the collided bullets */
	traverse(bullets->bullets, display_bullet);
}

