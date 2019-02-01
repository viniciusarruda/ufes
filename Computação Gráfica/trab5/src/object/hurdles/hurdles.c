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
#include "../../util/bmp_loader/bmp_loader.h"
#include "../../window/window.h"
#include "../../util/list/list.h"
#include "../../util/global/global.h"
#include "../../shape/circle/circle.h"
#include <GL/glut.h>
#include "hurdles.h"

typedef struct
{
	float radius;
	float height;
	float cx;
	float cy;
	float red;
	float green;
	float blue;
	GLuint wall_texture;
	GLuint floor_texture;
} Hurdle;

struct Hurdles_t
{
	float scale_high;
	List* hurdles;
};

static Hurdle* new_hurdle(float radius, float cx, float cy, float red, float green, float blue)
{
	Hurdle* new;
	get_memory(new, sizeof(Hurdle));
	new->height = 0.0;
	new->radius = radius;
	new->cx = cx;
	new->cy = cy;
	new->red = red;
	new->green = green;
	new->blue = blue;
	return new;
}

Hurdles* new_hurdles(void)
{
	Hurdles* new;
	get_memory(new, sizeof(Hurdles));
	new->hurdles = new_list();
	return new;
}


void set_percentage(Hurdles* hurdles, float percentage)
{
	hurdles->scale_high = 1.0 + 0.005 * percentage;
}

float hurdles_scale_high(Hurdles* hurdles)
{
	return hurdles->scale_high;
}

void add_hurdle(Hurdles* hurdles, float radius, float cx, float cy, float red, float green, float blue)
{
	append(hurdles->hurdles, new_hurdle(radius, cx, cy, red, green, blue)); 
}

static void adjust_hurdle(void* hurdle, void* arg)
{	
	void** arg_array = arg;
	cast(hurdle, Hurdle)->cy = adjust_y(cast(arg_array[0], Window), cast(hurdle, Hurdle)->cy);
	cast(hurdle, Hurdle)->height = *cast(arg_array[1], float);
	cast(hurdle, Hurdle)->wall_texture = *cast(arg_array[2], GLuint);
	cast(hurdle, Hurdle)->floor_texture = *cast(arg_array[3], GLuint);
}

void adjust_hurdles(Hurdles* hurdles, Window* window, float height)
{
	GLuint wall_texture, floor_texture;
	void* arg[] = {window, &height, &wall_texture, &floor_texture};

	wall_texture = LoadTexture("texture/wall_hurdle.bmp");
	floor_texture = LoadTexture("texture/floor_hurdle.bmp");

	traverse_arg(hurdles->hurdles, adjust_hurdle, arg);
}

static void delete_hurdle(void** hurdle)  
{
	free_memory(&cast_twice(hurdle, Hurdle));
}

void delete_hurdles(Hurdles** hurdles)
{
	delete_list(&(*hurdles)->hurdles, delete_hurdle);
	free_memory(hurdles);
}

static void display_mini_hurdle(void* hurdle)
{
	glPushMatrix();
		glTranslatef(cast(hurdle, Hurdle)->cx, cast(hurdle, Hurdle)->cy, 0.0);
		display_disk(cast(hurdle, Hurdle)->radius, cast(hurdle, Hurdle)->red, cast(hurdle, Hurdle)->green, cast(hurdle, Hurdle)->blue);
	glPopMatrix();
}

static void display_hurdle(void* hurdle)
{
	Hurdle* h = hurdle;
	GLUquadric* quad;

	quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, global_mat(NONE_mat));
    glMaterialfv(GL_FRONT, GL_SHININESS, global_mat(SHININESS_mat));
    glMaterialfv(GL_FRONT, GL_EMISSION, global_mat(NONE_mat));
    glMaterialfv(GL_FRONT, GL_AMBIENT, global_mat(LOW_mat));  
    glMaterialfv(GL_FRONT, GL_DIFFUSE, global_mat(HIGH_mat));


	glPushMatrix();   

		glTranslatef(h->cx, h->cy, 0.0);

		glBindTexture(GL_TEXTURE_2D, h->wall_texture);
		gluQuadricOrientation(quad, GLU_OUTSIDE);
		glMatrixMode(GL_TEXTURE); 
        glPushMatrix();        
        	glScalef(5, 0.5, 1);
        	gluCylinder(quad, h->radius, h->radius, h->height, HIGH_DENSITY_SLICES, HIGH_DENSITY_LOOPS);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);  

        glBindTexture(GL_TEXTURE_2D, h->floor_texture);
		gluQuadricOrientation(quad, GLU_OUTSIDE);
        glPushMatrix();        
        	glTranslatef(0.0, 0.0, h->height);
        	gluDisk(quad, 0.0, h->radius, HIGH_DENSITY_SLICES, HIGH_DENSITY_LOOPS);
        glPopMatrix();

    glPopMatrix();


    glBindTexture(GL_TEXTURE_2D, 0);

	gluDeleteQuadric(quad);	
}

void display_hurdles(Hurdles* hurdles)
{
	traverse(hurdles->hurdles, display_hurdle);
}

void display_mini_hurdles(Hurdles* hurdles)
{
	traverse(hurdles->hurdles, display_mini_hurdle);
}

static int collided_hurdle_generic(void* hurdle, void* arg)
{
	float* arg_array = arg;

	return collided_circle(cast(hurdle, Hurdle)->cx, cast(hurdle, Hurdle)->cy, cast(hurdle, Hurdle)->radius, arg_array[0], arg_array[1], arg_array[2]);
}

int collided_hurdles(Hurdles* hurdles, float cx, float cy, float radius)
{
	float arg[3]; 

	arg[0] = cx;
	arg[1] = cy;
	arg[2] = radius;

	return hurdles != NULL && traverse_true(hurdles->hurdles, collided_hurdle_generic, arg);
}

static int collided_bullet_hurdle_generic(void* hurdle, void* arg)
{
	float* arg_array = arg;

	return point_collided_cylinder(arg_array[0], arg_array[1], arg_array[2], cast(hurdle, Hurdle)->cx, cast(hurdle, Hurdle)->cy, 0.0, cast(hurdle, Hurdle)->height, cast(hurdle, Hurdle)->radius);
	//return collided_sphere(cast(hurdle, Hurdle)->cx, cast(hurdle, Hurdle)->cy, cast(hurdle, Hurdle)->cz, cast(hurdle, Hurdle)->radius, arg_array[0], arg_array[1], arg_array[2], arg_array[3]);
	//return collided_circle(cast(hurdle, Hurdle)->cx, cast(hurdle, Hurdle)->cy, cast(hurdle, Hurdle)->radius, arg_array[0], arg_array[1], arg_array[2]);
}

int collided_bullet_hurdles(Hurdles* hurdles, float cx, float cy, float cz)
{
	float arg[3]; 

	arg[0] = cx;
	arg[1] = cy;
	arg[2] = cz;

	return hurdles != NULL && traverse_true(hurdles->hurdles, collided_bullet_hurdle_generic, arg);
}


float get_hurdle_cx(void* hurdle)
{
	return cast(hurdle, Hurdle)->cx;
}

float get_hurdle_cy(void* hurdle)
{
	return cast(hurdle, Hurdle)->cy;
}

float get_hurdle_r(void* hurdle)
{
	return cast(hurdle, Hurdle)->radius;
}

void apply_for_all_hurdles(Hurdles* hurdles, void (* apply) (void *, void *), void* arg)
{
	traverse_arg(hurdles->hurdles, apply, arg);
}



