/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */

#include "../../application/application.h"
#include "../../util/util.h"
#include "../../window/window.h"
#include "../../util/list/list.h"
#include "../../util/math/math.h"
#include "../../util/time/time.h"
#include "../../shape/line/line.h"
#include "../../shape/circle/circle.h"
#include "../../shape/rectangle/rectangle.h"
#include "../hurdles/hurdles.h"
#include "../arena/arena.h"
#include "../enemies/enemies.h"
#include "../../scoreboard/scoreboard.h"
#include "character.h"
#include <math.h>
#include <GL/glut.h>


/* Values in [0.0, 1.0] (percentage in function of character speed) */

#define SPIN_RATE 0.6
#define GUN_SPIN_RATE 0.3
#define MAX_GUN_ANGLE 45.0

#define ANGLE_SENSOR 26.565051177
#define MULT_SENSOR_LENGTH 4.5

#define CODE_VOID 0
#define CODE_WALL_ENEMY 1
#define CODE_HURDLE 2
#define CODE_PLAYER 3

/***********************************/

struct Character_t
{
	Type type;
	int id;
	int dead;
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
	float gun_width;
	float gun_height;
	float gun_x;
	float gun_orientation;
	float gun_tip_x;
	float gun_tip_y;
	float bullet_speed;

	/* foot stuff */
	float foot_width;
	float foot_height;
	float foot_length;
	float ds_swap_foot;

	float base;
	float v0;

	/* jumping stuff */
	int jumping;         /* bool */
	int jump_time_elapsed;
	float scale_rate;
	int over_hurdle; /* bool */
	int was_over_hurdle;


	/* AI stuff */

	int consume_movement;

	float sensor_x0;
	float sensor_y0;
	float drawable_x0;
	float drawable_y0;
	int sensor0_code;

	float sensor_x1;
	float sensor_y1;
	float drawable_x1;
	float drawable_y1;
	int sensor1_code;

	float sensor_x2;
	float sensor_y2;
	float drawable_x2;
	float drawable_y2;
	int sensor2_code;
	
	int swap_movement;
	int rotate_state; 

	float cx_space;
	float cy_space;
	int dt_space;

	int is_wall_in_front_0;
	int is_wall_in_front_1;
	int is_wall_in_front_2;

	int dt_shoot;
};


static void update_sensor(Character* character)
{
	character->sensor_x0 = character->drawable_x0 = character->cx + cos_rad(character->angle + ANGLE_SENSOR) * MULT_SENSOR_LENGTH * character->radius;
	character->sensor_y0 = character->drawable_y0 = character->cy + sin_rad(character->angle + ANGLE_SENSOR) * MULT_SENSOR_LENGTH * character->radius;
	character->sensor_x1 = character->drawable_x1 = character->cx + cos_rad(character->angle) * MULT_SENSOR_LENGTH * character->radius;
	character->sensor_y1 = character->drawable_y1 = character->cy + sin_rad(character->angle) * MULT_SENSOR_LENGTH * character->radius;
	character->sensor_x2 = character->drawable_x2 = character->cx + cos_rad(character->angle - ANGLE_SENSOR) * MULT_SENSOR_LENGTH * character->radius;
	character->sensor_y2 = character->drawable_y2 = character->cy + sin_rad(character->angle - ANGLE_SENSOR) * MULT_SENSOR_LENGTH * character->radius;	
}


static void update_gun_tip(Character* character) 
{
	character->gun_tip_x = character->cx + sin_rad(character->angle) * character->gun_x + cos_rad(character->angle + character->gun_orientation) * character->gun_height;
	character->gun_tip_y = character->cy - cos_rad(character->angle) * character->gun_x + sin_rad(character->angle + character->gun_orientation) * character->gun_height;
}


Character* new_character(Type type, int id, float radius, float cx, float cy, float red, float green, float blue)
{
	Character* new;
	get_memory(new, sizeof(Character));

	new->type = type;
	new->id = id;
	new->dead = 0;
	new->cx = cx;
	new->cy = cy;
	new->red = red;
	new->green = green;
	new->blue = blue;
	new->jumping = 0;
	new->radius = radius;
	new->jump_time_elapsed = 0;
	new->walk_time_elapsed = 0;
	new->foot_width = radius * 0.3;
	new->foot_height = radius * 0.8;
	new->foot_length = 0.0;
	new->gun_width = radius * 0.2;
	new->gun_height = radius * 0.8;
	new->ds_swap_foot = 0.0;
	new->angle = 0.0;
	new->scale_rate = 1.0;

	new->gun_x = radius - new->gun_width;
	new->gun_orientation = to_angle(atanf(new->gun_x / (MULT_SENSOR_LENGTH * new->radius)));
	
	new->over_hurdle = 0;
	new->was_over_hurdle = 0;
	new->base = 1.0;

	/* from Center(0, 0) */
	new->sensor0_code = -1;
	new->sensor1_code = -1;
	new->sensor2_code = -1;
	new->consume_movement = 0;
	new->swap_movement = 1;
	new->rotate_state = 0;

	new->cx_space = 0.0; // will do random behavior at initial
	new->cy_space = 0.0;

	new->dt_space = 0;
	new->is_wall_in_front_0 = 0;
	new->is_wall_in_front_1 = 0;
	new->is_wall_in_front_2 = 0;

	new->dt_shoot = 0; /* just for enemies */

	return new;  
}

int in_center_space(Character* character)
{
	return point_inside_circle(character->cx_space, character->cy_space, character->radius / 2.0, character->cx, character->cy);
}

void reset_dt_shoot(Character* character)
{
	character->dt_shoot = glutGet(GLUT_ELAPSED_TIME);
}

int get_dt_shoot(Character* character)
{
	return character->dt_shoot;
}

void set_center_space(Character* character)
{
	character->cx_space = character->cx;
	character->cy_space = character->cy;
}

int get_dt_space(Character* character)
{
	return character->dt_space;
}

void reset_dt_space(Character* character)
{
	character->dt_space = glutGet(GLUT_ELAPSED_TIME);
}

int get_rotate_state(Character* character)
{
	return character->rotate_state;
}

void toggle_rotate_state(Character* character)
{
	character->rotate_state = !character->rotate_state;
}

int get_consume_movement(Character* character)
{
	return character->consume_movement;
}

void set_consume_movement(Character* character, int consume_movement)
{
	character->consume_movement = consume_movement;
}

int get_swap_movement(Character* character)
{
	return character->swap_movement;
}

void swap_movement(Character* character)
{
	character->swap_movement *= -1;
}


void delete_character(void** character)
{
	free_memory(&cast_twice(character, Character));
}

int bullet_collided_with_character_void(void* character, void* arg)
{
	void** arg_array = arg;
	return bullet_collided_with_character(character, *cast(arg_array[4], int), *cast(arg_array[3], int), *cast(arg_array[0], float), *cast(arg_array[1], float), *cast(arg_array[2], float), arg_array[5]);
}

int is_dead(void* character)
{
	return cast(character, Character)->dead;
}

int bullet_collided_with_character(Character* character, Type who_shot_type, int who_shot_id, float cx, float cy, float radius, Scoreboard* scoreboard)
{
	int ret = who_shot_id != character->id && collided_circle(character->cx, character->cy, character->radius, cx, cy, radius);

	if(ret && who_shot_type != character->type && !character->over_hurdle) 
	{
		character->dead = 1;

		if(character->type == ENEMY)
			enemy_killed(scoreboard);
		else if(character->type == PLAYER)
			player_killed(scoreboard);
	}

	return ret;
}

void set_speed(Character* character, float speed, float bullet_speed) 
{ 
	character->speed = speed; 
	character->bullet_speed = bullet_speed; 
}

void adjust_character_void(void* character, void* window)
{
	adjust_character(character, window);
}

void adjust_character(Character* character, Window* window)
{
	character->cy = adjust_y(window, character->cy);
	//character->gun_tip_y = character->cy + character->gun_height;
	update_gun_tip(character);
	update_sensor(character);
}

int get_sensor_0(Character* character)
{
	return character->sensor0_code;
}

int get_sensor_1(Character* character)
{
	return character->sensor1_code;
}

int get_sensor_2(Character* character)
{
	return character->sensor2_code;
}

void display_character(void* character) 
{
#ifdef DEBUG_SENSOR
	float r, g, b;
#endif
	Character* p = character;

	if(p->dead)
		return;

	glPushMatrix();   /* all in this order !!!! */

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

		glPushMatrix();
			glScalef(0.4, 0.5, 1.0);
			display_circle(p->radius, p->red, p->green, p->blue);
		glPopMatrix();

    glPopMatrix();

#ifdef DEBUG_SENSOR
    if(p->sensor1_code != -1)
    {
	    glPushMatrix();

	    	get_color_by_code(p->sensor0_code, p->is_wall_in_front_0, &r, &g, &b);
			display_line(p->cx, p->cy, p->drawable_x0, p->drawable_y0, r, g, b);

			glPointSize( 5.0 );
			glColor3f(r, g, b);
			glBegin( GL_POINTS );
		        glVertex2f(p->drawable_x0, p->drawable_y0);
		    glEnd();


	    	get_color_by_code(p->sensor1_code, p->is_wall_in_front_1, &r, &g, &b);
			display_line(p->cx, p->cy, p->drawable_x1, p->drawable_y1, r, g, b);

			glPointSize( 5.0 );
			glColor3f(r, g, b);
			glBegin( GL_POINTS );
		        glVertex2f(p->drawable_x1, p->drawable_y1);
		    glEnd();


		    get_color_by_code(p->sensor2_code, p->is_wall_in_front_2, &r, &g, &b);
			display_line(p->cx, p->cy, p->drawable_x2, p->drawable_y2, r, g, b);

			glPointSize( 5.0 );
			glColor3f(r, g, b);
			glBegin( GL_POINTS );
		        glVertex2f(p->drawable_x2, p->drawable_y2);
		    glEnd();

		glPopMatrix();
	}
#endif

}


int character_shooted(Character* character, int shoot)
{
	if(!character->jumping && !character->over_hurdle && shoot)
	{
		app_add_bullet(character->type, character->id, character->gun_tip_x, character->gun_tip_y, character->gun_orientation + character->angle, character->bullet_speed);
		return 1;
	}

	return 0;
}


int character_move_gun(Character* character, int dx)
{
	if(dx)
	{
		character->gun_orientation += dx * GUN_SPIN_RATE;

		if(character->gun_orientation > MAX_GUN_ANGLE) 
			character->gun_orientation = MAX_GUN_ANGLE;
		else if(character->gun_orientation < -MAX_GUN_ANGLE)
			character->gun_orientation = -MAX_GUN_ANGLE;

		update_gun_tip(character);
		
		return 1;
	}

	return 0;
}

int collided_with_character_void(void* character, void* arg)
{
	float* arg_array = arg;
	return collided_with_character(character, arg_array[0], arg_array[1], arg_array[2]);
}

int collided_with_character(Character* character, float cx, float cy, float radius)
{
	return collided_circle(character->cx, character->cy, character->radius, cx, cy, radius);
}

int get_character_id(Character* character) { return character->id; }

int not_me(void* character, void* id_void)
{
	return cast(character, Character)->id != *cast(id_void, int);
}


static void test_swap_foot(Character* character, float ds)
{
	if(!character->jumping)
	{
		character->ds_swap_foot += ds;
		character->foot_length = ((2.0 * character->foot_height) / PI) * asin(sin((TWO_PI / (4.0 * character->foot_height)) * character->ds_swap_foot));
	}
}


void character_jump(Character* character, int jump)
{
    if(jump && !character->jumping)
    {
		character->jumping = 1;
		character->jump_time_elapsed = glutGet(GLUT_ELAPSED_TIME);
		character->v0 = 1.0;
	}
}


static void set_jump_scale(Character* character, float floor)
{
	static float old_scale;
	float t = (glutGet(GLUT_ELAPSED_TIME) - character->jump_time_elapsed) / 1000.0;

	old_scale = character->scale_rate;
	character->scale_rate = character->base + character->v0 * t - 0.5 * t * t;

	if(character->scale_rate < old_scale && character->scale_rate < floor)
	{
		character->scale_rate = character->base = floor;
		character->jumping = 0;
	}


}

static int character_collided(Arena* arena, Hurdles* hurdles, float cx, float cy, float radius, int (* collided_extra) (void *, float, float, float), void* arg)
{
	return collided_arena(arena, cx, cy, radius)      ||
	       collided_hurdles(hurdles, cx, cy, radius)  || 
	       collided_extra(arg, cx, cy, radius);
}

int idle_character(Character* character, Arena* arena, Hurdles* hurdles, Enemies* enemies, Character* player, int up_down, int left_right, int (* collided_extra) (void *, float, float, float), void* arg)
{
	int dirty = 0;
	float ds = delta_time(&character->walk_time_elapsed) * character->speed;
	float x_component = cos_rad(character->angle) * ds;
	float y_component = sin_rad(character->angle) * ds;
	float floor = 1.0;
	void* save_hurdles = hurdles;

	character->over_hurdle = collided_hurdles(hurdles, character->cx, character->cy, character->radius);

	if(character->over_hurdle) // hovering
	{
		floor = hurdles_scale_high(hurdles); 
		hurdles = NULL; 
	}
	else if(!character->jumping && character->was_over_hurdle)
	{
		character->jumping = 1;
		character->v0 = 0.0;
		character->jump_time_elapsed = glutGet(GLUT_ELAPSED_TIME);
	}

	if(character->jumping)
	{
		dirty = 1;		
		set_jump_scale(character, floor);

		if(hurdles != NULL && character->scale_rate > hurdles_scale_high(hurdles))
		{
			hurdles = NULL;
		}
	}

	if(up_down == 1 && !character_collided(arena, hurdles, character->cx + x_component, character->cy + y_component, character->radius, collided_extra, arg))
	{
		test_swap_foot(character, ds);
        character->cx += x_component;
		character->cy += y_component;
		dirty = 1;
	}
	else if(up_down == -1 && !character_collided(arena, hurdles, character->cx - x_component, character->cy - y_component, character->radius, collided_extra, arg))
	{
		test_swap_foot(character, -ds);
        character->cx -= x_component;
		character->cy -= y_component;
		dirty = 1;
	}

    if(left_right == -1)
	{
        character->angle -= ds * SPIN_RATE; 
		dirty = 1;
	}
    else if(left_right == 1) 
	{
        character->angle += ds * SPIN_RATE;
		dirty = 1;
	}

	if(dirty)
	{
		update_gun_tip(character);
		update_sensor(character);

		// por ultimo
		detects_objects(character, arena, save_hurdles, enemies, player);
	}


	character->was_over_hurdle = character->over_hurdle;

   	return dirty;
}


/* ARTIFICIAL INTELLIGENCE */

// cx0 e cy0 é o centro de onde sai o sensor
int who_is_closer_to_sensor(float* draw_x, float* draw_y, float cx, float cy, float r, float cx0, float cy0, float x1, float y1) 
{
	if(line_intercepts_circle(cx, cy, r, cx0, cy0, x1, y1, draw_x, draw_y))
	{
		return 1;
	}

	*draw_x = x1;
	*draw_y = y1;

	return 0;
}


static void generic_sensor_hurdle(void* hurdle, void* arg)
{
	void** args = (void **) arg;

	if(!point_inside_circle(get_hurdle_cx(hurdle), get_hurdle_cy(hurdle), get_hurdle_r(hurdle), *((float *) args[2]), *(((float **)args)[3]))
	   && who_is_closer_to_sensor(((float **)args)[0], ((float **)args)[1], get_hurdle_cx(hurdle), get_hurdle_cy(hurdle), get_hurdle_r(hurdle), *((float *) args[2]), *(((float **)args)[3]), *(((float **)args)[4]), *(((float **)args)[5])))
		*(((int **)args)[6]) = *(((int **)args)[7]);	
}

static void generic_sensor_character(void* character, void* arg)
{
	void** args = (void **) arg;

	if(cast(character, Character)->id == *(((int **)args)[8]))
		return;

	if(who_is_closer_to_sensor(((float **)args)[0], ((float **)args)[1], cast(character, Character)->cx, cast(character, Character)->cy, cast(character, Character)->radius, *((float *) args[2]), *(((float **)args)[3]), *(((float **)args)[4]), *(((float **)args)[5])))
	{		
		*(((int **)args)[6]) = *(((int **)args)[7]);	
		*(((int **)args)[8]) = 0;
	}
}

void detects_objects(Character* c, Arena* arena, Hurdles* hurdles, Enemies* enemies, Character* player)
{
	int code0, code1, code2, id_obj;
	void* arg_0[] = {&c->drawable_x0, &c->drawable_y0, &c->cx, &c->cy, &c->drawable_x0, &c->drawable_y0, &c->sensor0_code, &code0, &id_obj, &c->is_wall_in_front_0};
	void* arg_1[] = {&c->drawable_x1, &c->drawable_y1, &c->cx, &c->cy, &c->drawable_x1, &c->drawable_y1, &c->sensor1_code, &code1, &id_obj, &c->is_wall_in_front_1};
	void* arg_2[] = {&c->drawable_x2, &c->drawable_y2, &c->cx, &c->cy, &c->drawable_x2, &c->drawable_y2, &c->sensor2_code, &code2, &id_obj, &c->is_wall_in_front_2};

	c->sensor0_code = c->sensor1_code = c->sensor2_code = 0;
	c->is_wall_in_front_0 = c->is_wall_in_front_1 = c->is_wall_in_front_2 = 0;


	code0 = CODE_WALL_ENEMY;
	id_obj = -1;

	if(who_is_closer_to_sensor(&c->drawable_x0, &c->drawable_y0, get_arena_inner_cx(arena), get_arena_inner_cy(arena), get_arena_inner_size(arena), c->cx, c->cy, c->sensor_x0, c->sensor_y0))
	{
		c->sensor0_code = code0;
		c->is_wall_in_front_0 = 1;
	}

	if(who_is_closer_to_sensor(&c->drawable_x0, &c->drawable_y0, get_arena_cx(arena), get_arena_cy(arena), get_arena_size(arena), c->cx, c->cy, c->drawable_x0, c->drawable_y0))
	{
		c->sensor0_code = code0;
		c->is_wall_in_front_0 = 1;
	}		

	//apply for all hurdles function
	code0 = CODE_HURDLE;
	id_obj = -1;
	apply_for_all_hurdles(hurdles, generic_sensor_hurdle, arg_0);


	code0 = CODE_WALL_ENEMY;
	id_obj = c->id;
	apply_for_all_enemies(enemies, generic_sensor_character, arg_0);


	code0 = CODE_PLAYER;
	id_obj = c->id;
	if(c->id != player->id && who_is_closer_to_sensor(&c->drawable_x0, &c->drawable_y0, player->cx, player->cy, player->radius, c->cx, c->cy, c->drawable_x0, c->drawable_y0))
		c->sensor0_code = code0;


	code1 = CODE_WALL_ENEMY;  /* in this order !!! */
	id_obj = -1;

	if(who_is_closer_to_sensor(&c->drawable_x1, &c->drawable_y1, get_arena_inner_cx(arena), get_arena_inner_cy(arena), get_arena_inner_size(arena), c->cx, c->cy, c->sensor_x1, c->sensor_y1))
	{
		c->sensor1_code = code1;
		c->is_wall_in_front_1 = 1;	
	}

	if(who_is_closer_to_sensor(&c->drawable_x1, &c->drawable_y1, get_arena_cx(arena), get_arena_cy(arena), get_arena_size(arena), c->cx, c->cy, c->drawable_x1, c->drawable_y1))
	{
		c->sensor1_code = code1;
		c->is_wall_in_front_1 = 1;
	}
	
	//apply for all hurdles function
	code1 = CODE_HURDLE;
	id_obj = -1;
	apply_for_all_hurdles(hurdles, generic_sensor_hurdle, arg_1);

	code1 = CODE_WALL_ENEMY;
	id_obj = c->id;
	apply_for_all_enemies(enemies, generic_sensor_character, arg_1);

	code1 = CODE_PLAYER;
	id_obj = c->id;
	if(c->id != player->id && who_is_closer_to_sensor(&c->drawable_x1, &c->drawable_y1, player->cx, player->cy, player->radius, c->cx, c->cy, c->drawable_x1, c->drawable_y1))
		c->sensor1_code = code1;


	code2 = CODE_WALL_ENEMY;
	id_obj = -1;
	if(who_is_closer_to_sensor(&c->drawable_x2, &c->drawable_y2, get_arena_inner_cx(arena), get_arena_inner_cy(arena), get_arena_inner_size(arena), c->cx, c->cy, c->sensor_x2, c->sensor_y2))
	{
		c->sensor2_code = code2;
		c->is_wall_in_front_2 = 1;
	}

	if(who_is_closer_to_sensor(&c->drawable_x2, &c->drawable_y2, get_arena_cx(arena), get_arena_cy(arena), get_arena_size(arena), c->cx, c->cy, c->drawable_x2, c->drawable_y2))
	{
		c->sensor2_code = code2;
		c->is_wall_in_front_2 = 1;
	}

	//apply for all hurdles function
	code2 = CODE_HURDLE;
	id_obj = -1;
	apply_for_all_hurdles(hurdles, generic_sensor_hurdle, arg_2);

	code2 = CODE_WALL_ENEMY;
	id_obj = c->id;
	apply_for_all_enemies(enemies, generic_sensor_character, arg_2);

	code2 = CODE_PLAYER;
	id_obj = c->id;
	if(c->id != player->id && who_is_closer_to_sensor(&c->drawable_x2, &c->drawable_y2, player->cx, player->cy, player->radius, c->cx, c->cy, c->drawable_x2, c->drawable_y2))
		c->sensor2_code = code2;
}


void get_color_by_code(int code, int is_wall_in_front, float* r, float* g, float* b) 
{
	switch(code)
	{
		case CODE_VOID:
			*r = 1;
			*g = 0;
			*b = 1;
			break;
		case CODE_WALL_ENEMY:
			if(is_wall_in_front)
			{
				*r = 1;
				*g = 1;
				*b = 1;
			}
			else
			{
				*r = 1;
				*g = 0;
				*b = 0;
			}
			break;
		case CODE_HURDLE:
			*r = 0;
			*g = 0;
			*b = 0;
			break;
		case CODE_PLAYER:
			*r = 0;
			*g = 1;
			*b = 0;
			break;
	}
}
