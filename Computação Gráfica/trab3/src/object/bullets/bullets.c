/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of shooting and improvements on moving.
 *
 */
 
#include "../../util/util.h"
#include "../../util/list/list.h"
#include "../../util/math/math.h"  
#include "../../shape/circle/circle.h"
#include "../../util/time/time.h"
#include "../arena/arena.h"
#include "../hurdle/hurdle.h"
#include <stdlib.h>
#include <GL/glut.h>
#include "bullets.h"

#define BULLET_RADIUS 4

typedef struct
{
	/* int id_who_shooted; */
	float cx;
	float cy;
	float cos_unitary;
	float sin_unitary;
	int time_last_update;
} Bullet;

struct Bullets_t
{
	List* bullets;
	float bullet_speed;
};

int has_bullets(Bullets* bullets)
{
	return get_size(bullets->bullets) != 0;
}

static Bullet* new_bullet(float cx, float cy, float orientation)
{
	Bullet* new;
	get_memory(new, sizeof(Bullet));
	new->cx = cx;
	new->cy = cy;
	new->cos_unitary = cos_rad(orientation);
	new->sin_unitary = sin_rad(orientation);
	new->time_last_update = glutGet(GLUT_ELAPSED_TIME);
	return new;
}

Bullets* new_bullets(float bullet_speed)
{
	Bullets* new;
	get_memory(new, sizeof(Bullets));
	new->bullets = new_list();
	new->bullet_speed = bullet_speed;
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

void add_bullet(Bullets* bullets, float cx, float cy, float orientation)
{
	append(bullets->bullets, new_bullet(cx, cy, orientation));
}

static void update_bullet(void* bullet, void* bullet_speed)
{
	float ds = delta_time(&cast(bullet, Bullet)->time_last_update) * *cast(bullet_speed, float);

	cast(bullet, Bullet)->cx += cast(bullet, Bullet)->cos_unitary * ds;
	cast(bullet, Bullet)->cy += cast(bullet, Bullet)->sin_unitary * ds;
}

static int collided_bullet(void* bullet, void* arg)
{
	Arena* arena = ((void **) arg)[0];
	List* small_hurdles = ((void **) arg)[1];
	List* big_hurdles = ((void **) arg)[2];

	return collided_arena(arena, cast(bullet, Bullet)->cx, cast(bullet, Bullet)->cy, BULLET_RADIUS) ||
	       collided_hurdle_list(big_hurdles, cast(bullet, Bullet)->cx, cast(bullet, Bullet)->cy, BULLET_RADIUS) ||
	       collided_hurdle_list(small_hurdles, cast(bullet, Bullet)->cx, cast(bullet, Bullet)->cy, BULLET_RADIUS);
}

static void display_bullet(void* bullet)
{
	glPushMatrix();
		glTranslatef(cast(bullet, Bullet)->cx, cast(bullet, Bullet)->cy, 0.0);
		display_circle(BULLET_RADIUS, DARK_GRAY);
	glPopMatrix();	
}

void display_bullets(Bullets* bullets, Arena* arena, List* small_hurdles, List* big_hurdles) /* colocar os hurdles */
{	
	void* arg[3] = {arena, small_hurdles, big_hurdles};

	traverse_arg(bullets->bullets, update_bullet, &bullets->bullet_speed);              /* update bullet position      */
	traverse_conditional_delete(bullets->bullets, collided_bullet, arg, delete_bullet); /* delete the collided bullets */
	traverse(bullets->bullets, display_bullet);
}

