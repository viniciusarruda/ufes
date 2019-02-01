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
#include "../../util/bmp_loader/bmp_loader.h"
#include "../../window/window.h"
#include "../../util/global/global.h"
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
#include <GL/gl.h>
#include <GL/glu.h>


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

static void test_swap_foot(Character* character, float ds);

static GLuint skin_texture;
static GLuint hair_texture;
static GLuint gun_texture;
static int player_light_on = 0;


struct Character_t
{
	Type type;
	int id;
	int dead;
	float cx;
	float cy;
	float radius;
	float height;
	float red;
	float green;
	float blue;
	float angle;

	/* walk stuff */
	float speed;
	int walk_time_elapsed;

	float angle_top_leg;
	float walk_swap_rate_top;
	float walk_swap_rate_bot;

	/* shoot stuff */
	float gun_x;
	float gun_horizontal_angle;
	float gun_vertical_angle;
	float gun_tip_x;
	float gun_tip_y;
	float gun_tip_z;
	float bullet_speed;

	float gun_cam_x;
	float gun_cam_y;
	float gun_cam_z;

	float gun_cam_xt;
	float gun_cam_yt;
	float gun_cam_zt;

	int swap_legs_xx;  // https://github.com/weidai11/cryptopp/issues/44

	/* foot stuff */
	float ds_swap_foot;

	float base;
	float v0;

	/* jumping stuff */
	int jumping;         /* bool */
	int jump_time_elapsed;
	float scale_rate;
	int over_hurdle; /* bool */
	int was_over_hurdle;

	GLuint texture;


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

static void run_once(void)
{
	static int runned = 0;

	if(runned) return;

	skin_texture = LoadTexture("texture/skin.bmp");
	hair_texture = LoadTexture("texture/hair.bmp");
	gun_texture = LoadTexture("texture/gun.bmp");
}


static void update_sensor(Character* character)
{
	character->sensor_x0 = character->drawable_x0 = character->cx + cos_rad(character->angle + ANGLE_SENSOR) * MULT_SENSOR_LENGTH * character->radius;
	character->sensor_y0 = character->drawable_y0 = character->cy + sin_rad(character->angle + ANGLE_SENSOR) * MULT_SENSOR_LENGTH * character->radius;
	character->sensor_x1 = character->drawable_x1 = character->cx + cos_rad(character->angle) * MULT_SENSOR_LENGTH * character->radius;
	character->sensor_y1 = character->drawable_y1 = character->cy + sin_rad(character->angle) * MULT_SENSOR_LENGTH * character->radius;
	character->sensor_x2 = character->drawable_x2 = character->cx + cos_rad(character->angle - ANGLE_SENSOR) * MULT_SENSOR_LENGTH * character->radius;
	character->sensor_y2 = character->drawable_y2 = character->cy + sin_rad(character->angle - ANGLE_SENSOR) * MULT_SENSOR_LENGTH * character->radius;	
}

void get_cam_tip_to(Character* p, float* x, float* y, float* z, float* xt, float* yt, float* zt)
{
	*x = p->gun_cam_x;
	*y = p->gun_cam_y;
	*z = p->gun_cam_z;

	*xt = p->gun_cam_xt;
	*yt = p->gun_cam_yt;
	*zt = p->gun_cam_zt;
}

static void update_gun_tips(Character* p) 
{
	float r, r_bullet;
                                                            // da arma em si
	r = p->radius * LEG_PART * 0.75 * 2.0 * cos_rad(45);//    + p->radius * LEG_PART * 0.25;
	r_bullet = r + p->radius * LEG_PART * 0.25;
                                                    // - + (inside)   + - (outside)

	p->gun_tip_x = p->gun_cam_xt = p->gun_cam_x = p->cx + sin_rad(p->angle) * p->gun_x;
	p->gun_tip_y = p->gun_cam_yt = p->gun_cam_y = p->cy - cos_rad(p->angle) * p->gun_x;

	p->gun_cam_xt = p->gun_cam_x +=  - sin_rad(p->gun_horizontal_angle + p->angle) * p->radius * ARMR_LOW * 2.0;
	p->gun_cam_yt = p->gun_cam_y +=  + cos_rad(p->gun_horizontal_angle + p->angle) * p->radius * ARMR_LOW * 2.0;
	p->gun_tip_z = p->gun_cam_zt = p->gun_cam_z = (p->scale_rate - 1.0) * p->height + p->radius * ( LEG_PART + LEG_PART + FULL_BODY_HEIGHT - BODY_HEIGHT_TOP);

	p->gun_cam_x += r * cos_rad(p->gun_horizontal_angle + p->angle) * sin_rad(p->gun_vertical_angle + 90.0);
	p->gun_cam_y += r * sin_rad(p->gun_horizontal_angle + p->angle) * sin_rad(p->gun_vertical_angle + 90.0);
	p->gun_cam_z += r * cos_rad(p->gun_vertical_angle + 90.0);

	p->gun_tip_x += r_bullet * cos_rad(p->gun_horizontal_angle + p->angle) * sin_rad(p->gun_vertical_angle + 90.0);
	p->gun_tip_y += r_bullet * sin_rad(p->gun_horizontal_angle + p->angle) * sin_rad(p->gun_vertical_angle + 90.0);
	p->gun_tip_z += r_bullet * cos_rad(p->gun_vertical_angle + 90.0);

	r += 1.0; /* mod 1 */

	p->gun_cam_xt += r * cos_rad(p->gun_horizontal_angle + p->angle) * sin_rad(p->gun_vertical_angle + 90.0);
	p->gun_cam_yt += r * sin_rad(p->gun_horizontal_angle + p->angle) * sin_rad(p->gun_vertical_angle + 90.0);
	p->gun_cam_zt += r * cos_rad(p->gun_vertical_angle + 90.0);
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
	new->height = radius * CHARACTER_HEIGHT;
	new->jump_time_elapsed = 0;
	new->walk_time_elapsed = 0;
	new->ds_swap_foot = 0.0;
	new->angle = 0.0;
	new->angle_top_leg = 0.0;
	new->scale_rate = 1.0;

	new->gun_x = (new->radius * 2.0 * 1.25 * LEGR_TOP);
	new->gun_horizontal_angle = to_angle(atanf(new->gun_x / (MULT_SENSOR_LENGTH * new->radius)));
	new->gun_vertical_angle = 0;//to_angle(atanf(new->gun_x / (MULT_SENSOR_LENGTH * new->radius)));  // ta errado isso ... ver o que foi feito no horizontal
	new->swap_legs_xx = 1;
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


static void set_light_character(Character* p)
{
	// float x, y, z, xt, yt, zt;
	GLfloat aux[] = {0, 0, 0, 1.0};

	if(p->type != PLAYER) return;

	// get_gun_tip_to(p, &x, &y, &z, &xt, &yt, &zt);

	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 8.0);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45);

	aux[0] = p->gun_cam_x;
	aux[1] = p->gun_cam_y;
	aux[2] = p->gun_cam_z;

	glLightfv(GL_LIGHT3, GL_POSITION, aux);

	aux[0] = p->gun_cam_xt - p->gun_cam_x;
	aux[1] = p->gun_cam_yt - p->gun_cam_y;
	aux[2] = p->gun_cam_zt - p->gun_cam_z;

	// printf("mod(%f)\n", sqrt(aux[0]*aux[0] + aux[1]*aux[1] + aux[2]*aux[2]));

	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, aux);
}

static void turn_off_gun_light(void)
{
	player_light_on = 0;
	glDisable(GL_LIGHT3);
}


static void turn_on_gun_light(void)
{	
	player_light_on = 1;
	glEnable(GL_LIGHT3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, global_mat(FULL_mat));
	glLightfv(GL_LIGHT3, GL_AMBIENT, global_mat(FULL_mat));
	glLightfv(GL_LIGHT3, GL_SPECULAR, global_mat(FULL_mat)); 
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.00001);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.00001);  // estudar esses parametros
}

void toggle_gun_light(Character* character)
{
	unused(character);

	if(player_light_on)
	{
		player_light_on = 0;
		turn_off_gun_light();
	}
	else
	{
		player_light_on = 1;
		turn_on_gun_light();
	}
}

float get_character_height(Character* character)
{
	return character->height;
}

float get_character_x(Character* character)
{
	return character->cx;
}

float get_character_y(Character* character)
{
	return character->cy;
}

float get_character_z(Character* character)
{
	return character->scale_rate * character->height;
}

float get_character_eye(Character* character)
{
	return character->radius * EYE_HEIGHT + (character->scale_rate - 1.0) * character->height;
}

float get_character_angle(Character* character)
{
	return character->angle;
}

float get_character_dist_camera(Character* character)
{
	return character->height;
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

int bullet_collided_with_character(Character* character, Type who_shot_type, int who_shot_id, float cx, float cy, float cz, Scoreboard* scoreboard)
{
	//int ret = who_shot_id != character->id && collided_circle(character->cx, character->cy, character->radius, cx, cy, radius);
 	int ret = who_shot_id != character->id && point_collided_cylinder(cx, cy, cz, character->cx, character->cy, (character->scale_rate - 1.0) * character->height, (character->scale_rate - 1.0) * character->height + character->height, character->radius);

	//if(ret && who_shot_type != character->type && !character->over_hurdle) 
	if(ret && who_shot_type != character->type) 
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

void set_texture(void* character, void* texture)
{
	cast(character, Character)->texture = *cast(texture, GLuint);
}

void adjust_character(Character* character, Window* window)
{
	character->cy = adjust_y(window, character->cy);

	update_gun_tips(character);
	update_sensor(character);

	if(character->type == PLAYER)
	{
		run_once();
		character->texture = LoadTexture("texture/player.bmp");
		config_camera_zero(window, get_character_x(character), get_character_y(character), get_character_eye(character), get_character_angle(character));
		config_camera_two(window, get_character_x(character), get_character_y(character), get_character_z(character), get_character_angle(character), get_character_dist_camera(character), 0, 0);
		config_camera_one(window, character);
		turn_off_gun_light();
	}

	test_swap_foot(character, 0);
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

void display_mini_character(void* character)
{
	if(cast(character, Character)->dead)
		return;

	glPushMatrix();
		glTranslatef(cast(character, Character)->cx, cast(character, Character)->cy, 0.0);
		display_disk(cast(character, Character)->radius, cast(character, Character)->red, cast(character, Character)->green, cast(character, Character)->blue);
	glPopMatrix();
}

void display_character(void* character) 
{
#ifdef DEBUG_SENSOR
	float r, g, b;
#endif
	Character* p = character;
	GLUquadric* quad;

	if(player_light_on && p->type == PLAYER)
		set_light_character(p);

	quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);

	if(p->dead)
		return;

	glMaterialfv(GL_FRONT, GL_SPECULAR, global_mat(NONE_mat));
    glMaterialfv(GL_FRONT, GL_SHININESS, global_mat(SHININESS_mat));
    glMaterialfv(GL_FRONT, GL_EMISSION, global_mat(NONE_mat));
    glMaterialfv(GL_FRONT, GL_AMBIENT, global_mat(LOW_mat));  
    glMaterialfv(GL_FRONT, GL_DIFFUSE, global_mat(HIGH_mat));


    glBindTexture(GL_TEXTURE_2D, p->texture);

	glPushMatrix();   /* all in this order !!!! */

		glTranslatef(p->cx, p->cy, (p->scale_rate - 1.0) * p->height);
		glRotatef(p->angle, 0.0, 0.0, 1.0);

		glPushMatrix();
			/* canela esquerda */
			glTranslatef(0, p->radius * LEGR, 0);

			glTranslatef(0, 0, p->radius * LEG_PART);
			glTranslatef(0, 0, p->radius * LEG_PART);
			glRotatef(-p->walk_swap_rate_top * ANGLE_COXA, 0, 1, 0);
			glTranslatef(0, 0, -p->radius * LEG_PART);

			if(p->swap_legs_xx == 0)
			{
				if(p->walk_swap_rate_bot * ANGLE_CANELA + ANGLE_CANELA <= ANGLE_CANELA)
					glRotatef(p->walk_swap_rate_bot * ANGLE_CANELA + ANGLE_CANELA, 0, 1, 0);
				else
					glRotatef(ANGLE_CANELA, 0, 1, 0);
			}
			glTranslatef(0, 0, -p->radius * LEG_PART);


			gluQuadricOrientation(quad, GLU_OUTSIDE);
			glPushMatrix();
				glTranslatef(0, 0, p->radius * LEG_PART);
	        	gluDisk(quad, 0,  p->radius * LEGR_MID, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
	        glPopMatrix();
	      	gluCylinder(quad, p->radius * LEGR_LOW, p->radius * LEGR_MID, p->radius * LEG_PART, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
	      	glPushMatrix();
				gluQuadricOrientation(quad, GLU_INSIDE);
	        	gluDisk(quad, 0,  p->radius * LEGR_LOW, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
	        glPopMatrix();
	    glPopMatrix();

	    glPushMatrix();
	    	/* canela direita */
			glTranslatef(0, -p->radius * LEGR, 0);

			glTranslatef(0, 0, p->radius * LEG_PART);
			glTranslatef(0, 0, p->radius * LEG_PART);
			glRotatef(p->walk_swap_rate_top * ANGLE_COXA, 0, 1, 0);
			glTranslatef(0, 0, -p->radius * LEG_PART);
			if(p->swap_legs_xx == 1)
			{
				if(p->walk_swap_rate_bot * ANGLE_CANELA + ANGLE_CANELA <= ANGLE_CANELA)
					glRotatef(p->walk_swap_rate_bot * ANGLE_CANELA + ANGLE_CANELA, 0, 1, 0);
				else
					glRotatef(ANGLE_CANELA, 0, 1, 0);
			}
			glTranslatef(0, 0, -p->radius * LEG_PART);


			gluQuadricOrientation(quad, GLU_OUTSIDE);
			glPushMatrix();
				glTranslatef(0, 0, p->radius * LEG_PART);
	        	gluDisk(quad, 0,  p->radius * LEGR_MID, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
	        glPopMatrix();
	      	gluCylinder(quad, p->radius * LEGR_LOW, p->radius * LEGR_MID, p->radius * LEG_PART, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
	      	glPushMatrix();
				gluQuadricOrientation(quad, GLU_INSIDE);
	        	gluDisk(quad, 0,  p->radius * LEGR_LOW, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
	        glPopMatrix();
	    glPopMatrix();

	    glPushMatrix();
	    	glTranslatef(0, 0, p->radius * LEG_PART);

	    	glPushMatrix(); /* coxa esquerda */
				glTranslatef(0, p->radius * LEGR, 0);

				glTranslatef(0, 0, p->radius * LEG_PART);
				glRotatef(-p->walk_swap_rate_top * ANGLE_COXA, 0, 1, 0);
				glTranslatef(0, 0, -p->radius * LEG_PART);

				gluQuadricOrientation(quad, GLU_OUTSIDE);
				glPushMatrix();
					glTranslatef(0, 0, p->radius * LEG_PART);
		        	gluDisk(quad, 0,  p->radius * LEGR_TOP, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
		        glPopMatrix();
		      	gluCylinder(quad, p->radius * LEGR_MID, p->radius * LEGR_TOP, p->radius * LEG_PART, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
		      	glPushMatrix();
					gluQuadricOrientation(quad, GLU_INSIDE);
		        	gluDisk(quad, 0,  p->radius * LEGR_MID, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
		        glPopMatrix();
		    glPopMatrix();

		    glPushMatrix(); /* coxa direita */
				glTranslatef(0, -p->radius * LEGR, 0);

				// if(p->type == PLAYER)
				// 	printf("(%f)\n", p->walk_swap_rate_top);

				glTranslatef(0, 0, p->radius * LEG_PART);
				glRotatef(p->walk_swap_rate_top * ANGLE_COXA, 0, 1, 0);
				glTranslatef(0, 0, -p->radius * LEG_PART);

				gluQuadricOrientation(quad, GLU_OUTSIDE);
				glPushMatrix();
					glTranslatef(0, 0, p->radius * LEG_PART);
		        	gluDisk(quad, 0,  p->radius * LEGR_TOP, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
		        glPopMatrix();
		      	gluCylinder(quad, p->radius * LEGR_MID, p->radius * LEGR_TOP, p->radius * LEG_PART, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
		      	glPushMatrix();
					gluQuadricOrientation(quad, GLU_INSIDE);
		        	gluDisk(quad, 0,  p->radius * LEGR_MID, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
		        glPopMatrix();
		    glPopMatrix();


		    glPushMatrix();
		    	glTranslatef(0, 0, p->radius * LEG_PART);

		     	glPushMatrix(); /* tronco top */
		    		glTranslatef(0, 0, p->radius * (BODY_HEIGHT_MID + BODY_HEIGHT_LOW));
		        	glScalef(p->radius * LEGR_TOP, p->radius * 2.0 * LEGR_TOP, 1.0);
		        	gluQuadricOrientation(quad, GLU_OUTSIDE);
		        	
		        	glBindTexture(GL_TEXTURE_2D, skin_texture);
		        	glPushMatrix();
		        		glTranslatef(0, 0, p->radius * BODY_HEIGHT_TOP);
		        		gluDisk(quad, 0, 0.6, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
		        	glPopMatrix();
		        	glBindTexture(GL_TEXTURE_2D, p->texture);

		        	glMatrixMode(GL_TEXTURE); 
			        glPushMatrix();        
			        	glScalef(5, 0.5, 1);
			        	gluCylinder(quad, 1.25, 0.6, p->radius * BODY_HEIGHT_TOP, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
			        glPopMatrix();
			        glMatrixMode(GL_MODELVIEW); 

		        glPopMatrix();

		    	glPushMatrix(); /* tronco mid */
		    		glTranslatef(0, 0, p->radius * BODY_HEIGHT_LOW);
		        	glScalef(p->radius * LEGR_TOP, p->radius * 2.0 * LEGR_TOP, p->radius * BODY_HEIGHT_MID);
		        	gluQuadricOrientation(quad, GLU_OUTSIDE);
					
					glMatrixMode(GL_TEXTURE); 
			        glPushMatrix();        
			        	glScalef(5, 0.5, 1);
			        	gluCylinder(quad, 0.9, 1.25, 1.0, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
			        glPopMatrix();
			        glMatrixMode(GL_MODELVIEW); 

		        glPopMatrix();

		    	glPushMatrix(); /* tronco low */
		        	glScalef(p->radius * LEGR_TOP, p->radius * 2.0 * LEGR_TOP, 1.0);
		        	gluQuadricOrientation(quad, GLU_OUTSIDE);

		        	glMatrixMode(GL_TEXTURE); 
			        glPushMatrix();        
			        	glScalef(5, 0.5, 1);
			        	gluCylinder(quad, 1.05, 0.9, p->radius * BODY_HEIGHT_LOW, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
			        glPopMatrix();
			        glMatrixMode(GL_MODELVIEW); 
		        	
		        	glPushMatrix();
		        		gluQuadricOrientation(quad, GLU_INSIDE);
		        		gluDisk(quad, 0, 1.05, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
		        	glPopMatrix();
		        glPopMatrix();

		        glPushMatrix();
		        	glTranslatef(0, 0, p->radius * FULL_BODY_HEIGHT);

		    		glPushMatrix(); /* braco direito */
		        		glTranslatef(0, - (p->radius * 2.0 * 1.25 * LEGR_TOP), p->radius * -BODY_HEIGHT_TOP);
						
		        		glRotatef(p->gun_horizontal_angle, 0, 0, 1);
						glRotatef(p->gun_vertical_angle, 0, 1, 0);	        		

		        		glRotatef(-90, 0, 1, 0);	

		        		glBindTexture(GL_TEXTURE_2D, gun_texture);
	        			glPushMatrix(); /* arma */
	        				// len arma: p->radius * LEG_PART * 0.75 * 2.0 * cos_rad(45)
			        		glTranslatef(0, 0, -p->radius * LEG_PART * 0.75 * 2.0 * cos_rad(45) + p->radius * LEG_PART * 0.25);
			        		glRotatef(180, 1, 0, 0);

			        		gluQuadricOrientation(quad, GLU_OUTSIDE);
			        		glPushMatrix();
								glTranslatef(0, 0, p->radius * LEG_PART * 0.5);
					        	gluDisk(quad, 0,  p->radius * ARMR_LOW, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
					        glPopMatrix();
					      	gluCylinder(quad, p->radius * ARMR_LOW, p->radius * ARMR_LOW, p->radius * LEG_PART * 0.5, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
					      	glPushMatrix();
								gluQuadricOrientation(quad, GLU_INSIDE);
					        	gluDisk(quad, 0,  p->radius * ARMR_LOW, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
					        glPopMatrix();

					    glPopMatrix();


					    glBindTexture(GL_TEXTURE_2D, p->texture);

		        		glPushMatrix();
			        		glRotatef(-90, 0, 0, 1);
			        		glRotatef(-45, 1, 0, 0);

			        		glPushMatrix(); /* braco top */
				        		glRotatef(180, 1, 0, 0);
				        		gluQuadricOrientation(quad, GLU_OUTSIDE);
				        		glPushMatrix();
									glTranslatef(0, 0, p->radius * LEG_PART * 0.75);
						        	gluDisk(quad, 0,  p->radius * ARMR_MID, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
						        glPopMatrix();
						      	gluCylinder(quad, p->radius * ARMR_TOP, p->radius * ARMR_MID, p->radius * LEG_PART * 0.75, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
						      	glPushMatrix();
									gluQuadricOrientation(quad, GLU_INSIDE);
						        	gluDisk(quad, 0,  p->radius * ARMR_TOP, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
						        glPopMatrix();
						    glPopMatrix();

						    glPushMatrix(); /* ante braco */
						    	glTranslatef(0, 0, -p->radius * LEG_PART * 0.75);
				        		glRotatef(90, 1, 0, 0);
				        		glRotatef(180, 1, 0, 0);
				        		gluQuadricOrientation(quad, GLU_OUTSIDE);
				        		glPushMatrix();
									glTranslatef(0, 0, p->radius * LEG_PART * 0.75);
						        	gluDisk(quad, 0,  p->radius * ARMR_LOW, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
						        glPopMatrix();
						      	gluCylinder(quad, p->radius * ARMR_MID, p->radius * ARMR_LOW, p->radius * LEG_PART * 0.75, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
						      	glPushMatrix();
									gluQuadricOrientation(quad, GLU_INSIDE);
						        	gluDisk(quad, 0,  p->radius * ARMR_MID, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
						        glPopMatrix();
						    glPopMatrix();
						glPopMatrix();

		        	glPopMatrix();

		        	glPushMatrix();
		        		/* braco esquerdo */
		        		glTranslatef(0, + (p->radius * 2.0 * 1.25 * LEGR_TOP), p->radius * -BODY_HEIGHT_TOP);
		        		glRotatef(5, 1, 0, 0);

		        		glPushMatrix(); /* braco top */
			        		glRotatef(180, 1, 0, 0);
			        		gluQuadricOrientation(quad, GLU_OUTSIDE);
			        		glPushMatrix();
								glTranslatef(0, 0, p->radius * LEG_PART * 0.75);
					        	gluDisk(quad, 0,  p->radius * ARMR_MID, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
					        glPopMatrix();
					      	gluCylinder(quad, p->radius * ARMR_TOP, p->radius * ARMR_MID, p->radius * LEG_PART * 0.75, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
					      	glPushMatrix();
								gluQuadricOrientation(quad, GLU_INSIDE);
					        	gluDisk(quad, 0,  p->radius * ARMR_TOP, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
					        glPopMatrix();
					    glPopMatrix();

					    glPushMatrix(); /* ante braco */
					    	glTranslatef(0, 0, -p->radius * LEG_PART * 0.75);
					    	glRotatef(-50, 0.5, 1.0, 0);
			        		glRotatef(180, 1, 0, 0);
			        		gluQuadricOrientation(quad, GLU_OUTSIDE);
			        		glPushMatrix();
								glTranslatef(0, 0, p->radius * LEG_PART * 0.75);
					        	gluDisk(quad, 0,  p->radius * ARMR_LOW, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
					        glPopMatrix();
					      	gluCylinder(quad, p->radius * ARMR_MID, p->radius * ARMR_LOW, p->radius * LEG_PART * 0.75, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
					      	glPushMatrix();
								gluQuadricOrientation(quad, GLU_INSIDE);
					        	gluDisk(quad, 0,  p->radius * ARMR_MID, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
					        glPopMatrix();
					    glPopMatrix();

		        	glPopMatrix();


		        	glPushMatrix();
		        		/* cabeca */
		        		glTranslatef(0, 0, p->radius * NECK_HEIGHT);
		        		glScalef(p->radius * 0.1, p->radius * 0.1, 1.0);

		        		gluQuadricOrientation(quad, GLU_OUTSIDE);
		        		glBindTexture(GL_TEXTURE_2D, hair_texture);
		        		glPushMatrix();
		        			glTranslatef(0, 0, p->radius * HEAD_HEIGHT);
		        			gluDisk(quad, 0, 2.7, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
		        		glPopMatrix();

		        		glBindTexture(GL_TEXTURE_2D, skin_texture);
		        		gluCylinder(quad, 2.0, 2.7, p->radius * HEAD_HEIGHT, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);

		        		gluQuadricOrientation(quad, GLU_INSIDE);
						gluDisk(quad, 0, 2.0, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
	
		        	glPopMatrix();

		        	glBindTexture(GL_TEXTURE_2D, skin_texture);

		        	glPushMatrix();
						/* pescoco */
			        	glScalef(p->radius * 0.1, p->radius * 0.1, p->radius * NECK_HEIGHT);
			        	gluQuadricOrientation(quad, GLU_OUTSIDE);
			        	gluCylinder(quad, 1.0, 1.0, 1.0, LOW_DENSITY_SLICES, LOW_DENSITY_LOOPS);
			        glPopMatrix();

			         glBindTexture(GL_TEXTURE_2D, p->texture);

		        glPopMatrix();

		    glPopMatrix();

	    glPopMatrix();

    glPopMatrix();


    glBindTexture(GL_TEXTURE_2D, 0);



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

	//glBindTexture (GL_TEXTURE_2D, 0);

	gluDeleteQuadric(quad);
}


int character_shooted(Character* character, int shoot)
{
	//if(!character->jumping && !character->over_hurdle && shoot)
	if(shoot)
	{
		app_add_bullet(character->type, character->id, character->gun_tip_x, character->gun_tip_y, character->gun_tip_z, character->gun_cam_xt - character->gun_cam_x, character->gun_cam_yt - character->gun_cam_y, character->gun_cam_zt - character->gun_cam_z, character->bullet_speed);
		return 1;
	}

	return 0;
}


int character_move_gun(Character* character, int dx, int dy)
{
	int dirty = 0;

	if(dx)
	{
		character->gun_horizontal_angle += dx * GUN_SPIN_RATE;

		if(character->gun_horizontal_angle > MAX_GUN_ANGLE) 
			character->gun_horizontal_angle = MAX_GUN_ANGLE;
		else if(character->gun_horizontal_angle < -MAX_GUN_ANGLE)
			character->gun_horizontal_angle = -MAX_GUN_ANGLE;

		dirty = 1;
	}


	if(dy)
	{
		character->gun_vertical_angle += dy * GUN_SPIN_RATE;

		if(character->gun_vertical_angle > MAX_GUN_ANGLE) 
			character->gun_vertical_angle = MAX_GUN_ANGLE;
		else if(character->gun_vertical_angle < -MAX_GUN_ANGLE)
			character->gun_vertical_angle = -MAX_GUN_ANGLE;

		dirty = 1;
	}

	if(dirty)
		update_gun_tips(character);

	return dirty;
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
	float aux;

	if(!character->jumping)
	{
		character->ds_swap_foot += ds;

		aux = character->walk_swap_rate_top;
		character->walk_swap_rate_top = (2.0 / PI) * asin(sin((TWO_PI / (1.0 * character->height)) * (character->ds_swap_foot + 0.25 * character->height)));
		
		if(ds > 0.0)
			character->swap_legs_xx = (aux - character->walk_swap_rate_top) > 0.0;
		else
			character->swap_legs_xx = (aux - character->walk_swap_rate_top) < 0.0;

		character->walk_swap_rate_bot = (2.0 / PI) * asin(sin((TWO_PI / (0.5 * character->height)) * (character->ds_swap_foot - 0.125 * character->height)));
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
		update_gun_tips(character);
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


