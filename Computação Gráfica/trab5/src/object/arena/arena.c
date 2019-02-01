/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */

#include "../../util/math/math.h" 
#include "../../util/util.h"
#include "../../util/global/global.h"
#include "../../util/bmp_loader/bmp_loader.h"
#include "../../window/window.h"
#include "../../shape/circle/circle.h"
#include "arena.h"
#include <GL/glut.h>

struct Arena_t
{
	int light_on;

	float inner_radius;
	float inner_cx;
	float inner_cy;
	float inner_red;
	float inner_green;
	float inner_blue;

	float outer_radius;
	float outer_cx;
	float outer_cy;
	float outer_red;
	float outer_green;
	float outer_blue;

	float height;

	GLuint floor_texture;
	GLuint ceiling_texture;
	GLuint inner_texture;
	GLuint outer_texture;
	GLuint light_texture;

	GLfloat light_position[4];
};

void delete_arena(void** arena)
{
	free_memory(&cast_twice(arena, Arena));
}

static void light_base(void)
{
	glBegin(GL_POLYGON);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0);
		glVertex3f(-1.0, -0.5, 0.0);
		
		glNormal3f(0,0,1);
		glTexCoord2f(1,0);
		glVertex3f(1.0, -0.5, 0.0);
		
		glNormal3f(0,0,1);
		glTexCoord2f(1,1);
		glVertex3f(1.0, 0.5, 0.0);
		
		glNormal3f(0,0,1);
		glTexCoord2f(0,1);
		glVertex3f(-1.0, 0.5, 0.0);
	glEnd();
}

static void display_light(Arena* arena)
{
	if(arena->light_on)
	{
		glMaterialfv(GL_FRONT, GL_SPECULAR, global_mat(FULL_mat));
	    glMaterialfv(GL_FRONT, GL_SHININESS, global_mat(SHININESS_mat));
	    glMaterialfv(GL_FRONT, GL_EMISSION, global_mat(FULL_mat));
	    glMaterialfv(GL_FRONT, GL_AMBIENT, global_mat(FULL_mat));    
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, global_mat(FULL_mat));
	}
	else
	{
	    glMaterialfv(GL_FRONT, GL_SPECULAR, global_mat(NONE_mat));
	    glMaterialfv(GL_FRONT, GL_SHININESS, global_mat(SHININESS_mat));
	    glMaterialfv(GL_FRONT, GL_EMISSION, global_mat(NONE_mat));
	    glMaterialfv(GL_FRONT, GL_AMBIENT, global_mat(LOW_mat));  
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, global_mat(HIGH_mat));
	}

    glBindTexture (GL_TEXTURE_2D, arena->light_texture);

	glPushMatrix();
		glRotatef(180, 1,0,0);

		glTranslatef(0, 0, 0.25);

		light_base();

		glPushMatrix();
			glTranslatef(0, -0.5, -0.25/2.0 );
			glRotatef(90, 1,0,0);
			glScalef(1,0.25,1);
			light_base();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0, +0.5, -0.25/2.0 );
			glRotatef(-90, 1,0,0);
			glScalef(1,0.25,1);
			light_base();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(1.0, 0, -0.25/2.0 );
			glRotatef(90, 0,1,0);
			glScalef(0.25/2.0, 1 ,1);
			light_base();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-1.0, 0, -0.25/2.0 );
			glRotatef(-90, 0,1,0);
			glScalef(0.25/2.0, 1 ,1);
			light_base();
		glPopMatrix();
	glPopMatrix();
}


static void turn_off_ceiling_light(void)
{
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
}


static void turn_on_ceiling_light(void)
{	
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, global_mat(FULL_mat));
	glLightfv(GL_LIGHT0, GL_AMBIENT, global_mat(FULL_mat));
	glLightfv(GL_LIGHT0, GL_SPECULAR, global_mat(FULL_mat)); 
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.00001);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00001);  // estudar esses parametros

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, global_mat(FULL_mat));
	glLightfv(GL_LIGHT1, GL_AMBIENT, global_mat(FULL_mat));
	glLightfv(GL_LIGHT1, GL_SPECULAR, global_mat(FULL_mat)); 
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.00001);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.00001);  // estudar esses parametros

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, global_mat(FULL_mat));
	glLightfv(GL_LIGHT2, GL_AMBIENT, global_mat(FULL_mat));
	glLightfv(GL_LIGHT2, GL_SPECULAR, global_mat(FULL_mat)); 
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.00001);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.00001);  // estudar esses parametros
}

static void set_light_arena(Arena* arena)
{
	 glPushMatrix();
		glTranslatef(arena->outer_cx, arena->outer_cy, 0.0);
		glLightfv(GL_LIGHT0, GL_POSITION, arena->light_position);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(arena->outer_cx, arena->outer_cy, 0.0);
		glRotatef(120, 0, 0, 1);
		glLightfv(GL_LIGHT1, GL_POSITION, arena->light_position);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(arena->outer_cx, arena->outer_cy, 0.0);
		glRotatef(240, 0, 0, 1);
		glLightfv(GL_LIGHT2, GL_POSITION, arena->light_position);
	glPopMatrix();
}


void toggle_ceiling_light(Arena* arena)
{
	if(arena->light_on)
	{
		arena->light_on = 0;
		turn_off_ceiling_light();
	}
	else
	{
		arena->light_on = 1;
		turn_on_ceiling_light();
	}
}


void display_mini_arena(Arena* arena)
{
	glPushMatrix();

		glTranslatef(arena->outer_cx, arena->outer_cy, 0.0);
		display_circle(arena->inner_radius, arena->inner_red, arena->inner_green, arena->inner_blue);
		display_circle(arena->outer_radius, arena->outer_red, arena->outer_green, arena->outer_blue);

	glPopMatrix();  
}


void display_arena(void* arena)
{
	Arena* a = arena;
	GLUquadric* quad;

	if(a->light_on)
		set_light_arena(arena);

	quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, global_mat(NONE_mat));
    glMaterialfv(GL_FRONT, GL_SHININESS, global_mat(SHININESS_mat));
    glMaterialfv(GL_FRONT, GL_EMISSION, global_mat(NONE_mat));
    glMaterialfv(GL_FRONT, GL_AMBIENT, global_mat(LOW_mat));  
    glMaterialfv(GL_FRONT, GL_DIFFUSE, global_mat(HIGH_mat));


	glPushMatrix();

		glTranslatef(cast(arena, Arena)->outer_cx, cast(arena, Arena)->outer_cy, 0.0);

		glBindTexture(GL_TEXTURE_2D, a->floor_texture);
		gluQuadricOrientation(quad, GLU_OUTSIDE);
        glMatrixMode(GL_TEXTURE); 
        glPushMatrix();        
        	glScalef(3, 3, 1);
        	gluDisk(quad, a->inner_radius, a->outer_radius, HIGH_DENSITY_SLICES, HIGH_DENSITY_LOOPS);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);  


		glBindTexture(GL_TEXTURE_2D, a->outer_texture);
		gluQuadricOrientation(quad, GLU_INSIDE);
		glMatrixMode(GL_TEXTURE); 
        glPushMatrix();        
        	glScalef(5, 1, 1);
        	gluCylinder(quad, a->outer_radius, a->outer_radius, a->height, HIGH_DENSITY_SLICES, HIGH_DENSITY_LOOPS);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);  


		glBindTexture(GL_TEXTURE_2D, a->inner_texture);
		gluQuadricOrientation(quad, GLU_OUTSIDE);
		glMatrixMode(GL_TEXTURE); 
	        glPushMatrix();        
	        	glScalef(2, 1, 1);
	        	gluCylinder(quad, a->inner_radius, a->inner_radius, a->height, HIGH_DENSITY_SLICES, HIGH_DENSITY_LOOPS);
	        glPopMatrix();
        glMatrixMode(GL_MODELVIEW); 


        glBindTexture(GL_TEXTURE_2D, a->ceiling_texture);
        gluQuadricOrientation(quad, GLU_INSIDE);
        glPushMatrix();

        	glTranslatef(0.0, 0.0, a->height);

	        glMatrixMode(GL_TEXTURE); 
	        glPushMatrix();        
	        	glScalef(3, 3, 1);
	        	gluDisk(quad, a->inner_radius, a->outer_radius, HIGH_DENSITY_SLICES, HIGH_DENSITY_LOOPS);
	        glPopMatrix();
	        glMatrixMode(GL_MODELVIEW);

	    glPopMatrix();  


		// colocar mais 2 lampadas apenas.. 
		// implementar funcao de desligalas.. 
		// criar um botao toggle keyboard 'n' para ligar e desligalas

	    // todo 
		glBindTexture(GL_TEXTURE_2D, a->light_texture);
		glPushMatrix();
	    	glTranslatef(0, cast(arena, Arena)->inner_radius + (cast(arena, Arena)->outer_radius - cast(arena, Arena)->inner_radius) / 2.0, cast(arena, Arena)->height - 3.0);
	    	glScalef(50,50,50); // parametrizar esses valores proporcionais ao tamanho da arena 
	    	display_light(arena);
	    glPopMatrix();

	    glPushMatrix();
	    	glRotatef(120, 0, 0, 1);
	    	glTranslatef(0, cast(arena, Arena)->inner_radius + (cast(arena, Arena)->outer_radius - cast(arena, Arena)->inner_radius) / 2.0, cast(arena, Arena)->height - 3.0);
	    	glScalef(50,50,50);
	    	display_light(arena);
	    glPopMatrix();

	    glPushMatrix();
	    	glRotatef(240, 0, 0, 1);
	    	glTranslatef(0, cast(arena, Arena)->inner_radius + (cast(arena, Arena)->outer_radius - cast(arena, Arena)->inner_radius) / 2.0, cast(arena, Arena)->height - 3.0);
	    	glScalef(50,50,50);
	    	display_light(arena);
	    glPopMatrix();

	glPopMatrix();


	glBindTexture (GL_TEXTURE_2D, 0);

	gluDeleteQuadric(quad);
}

Arena* new_arena(float inner_radius, float inner_cx, float inner_cy, float inner_red, float inner_green, float inner_blue,
	             float outer_radius, float outer_cx, float outer_cy, float outer_red, float outer_green, float outer_blue)
{
	Arena* new;
	get_memory(new, sizeof(Arena));
	
	new->light_on = 1;

	new->inner_radius = inner_radius;
	new->inner_cx = inner_cx;
	new->inner_cy = inner_cy;
	new->inner_red = inner_red;
	new->inner_green = inner_green;
	new->inner_blue = inner_blue;

	new->outer_radius = outer_radius;
	new->outer_cx = outer_cx;
	new->outer_cy = outer_cy;
	new->outer_red = outer_red;
	new->outer_green = outer_green;
	new->outer_blue = outer_blue;

	new->height = 0.0;

	return new;
}

void adjust_arena(Arena* arena, Window* window, float height)
{
	arena->inner_cy = adjust_y(window, arena->inner_cy);
	arena->outer_cy = adjust_y(window, arena->outer_cy);

	arena->height = height;

	arena->floor_texture = LoadTexture("texture/floor_arena.bmp");
	arena->ceiling_texture = LoadTexture("texture/ceiling_arena.bmp");
	arena->inner_texture = LoadTexture("texture/inner_arena.bmp");
	arena->outer_texture = LoadTexture("texture/outer_arena.bmp");
	arena->light_texture = LoadTexture("texture/light_arena.bmp");

	arena->light_position[0] = 0.0;
	arena->light_position[1] = arena->inner_radius + (arena->outer_radius - arena->inner_radius) / 2.0;
	arena->light_position[2] = arena->height - 3.0;
	arena->light_position[3] = 1.0;

	turn_on_ceiling_light();
}

float get_arena_size(Arena* arena)
{
	return arena->outer_radius;
}

float get_arena_inner_size(Arena* arena)
{
	return arena->inner_radius;
}

float get_arena_inner_cx(Arena* arena)
{
	return arena->inner_cx;
}

float get_arena_inner_cy(Arena* arena)
{
	return arena->inner_cy;
}

float get_arena_cx(Arena* arena)
{
	return arena->outer_cx;
}

float get_arena_cy(Arena* arena)
{
	return arena->outer_cy;
}

int collided_arena(Arena* arena, float cx, float cy, float radius)
{
	return collided_circle(arena->inner_cx, arena->inner_cy, arena->inner_radius, cx, cy, radius) 
	       || !is_inside(cx, cy, radius, arena->outer_cx, arena->outer_cy, arena->outer_radius);
}
