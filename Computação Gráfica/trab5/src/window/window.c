/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#include "../util/util.h"
#include "../util/math/math.h"
#include "../util/global/global.h"
#include "window.h"
#include "../object/character/character.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

struct Window_t
{
	float cx;
	float cy;
	float size;
	const char* title;

	int which_camera;

	/* camera 0 */
	float x_zero;
	float y_zero;
	float z_zero;
	float to_x_zero;
	float to_y_zero;
	float to_z_zero;

	/* camera 1 */
	float x_one;
	float y_one;
	float z_one;
	float to_x_one;
	float to_y_one;
	float to_z_one;

	/* camera 2 */
	float x_two;
	float y_two;
	float z_two;
	float to_x_two;
	float to_y_two;
	float to_z_two;
	float horizontal_angle_two;
	float vertical_angle_two;
};


void init_window(Window* window)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_TOP_HEIGHT); 
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WINDOW_WIDTH) / 2.0, (glutGet(GLUT_SCREEN_HEIGHT) - WINDOW_TOP_HEIGHT) / 2.0);
	glutCreateWindow(window->title);
	glClearColor(MAX_COLOR, MAX_COLOR, MAX_COLOR, 0.0);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LEQUAL); // no z-fighting

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}	

Window* new_window(float cx, float cy, float size, const char* title) 
{
	Window* new;
	get_memory(new, sizeof(Window));
	new->cx = cx;
	new->cy = cy;
	new->size = size;
	new->title = title;

	init_window(new);

	new->horizontal_angle_two = HORIZONTAL_CAMERA_TWO;
	new->vertical_angle_two = VERTICAL_CAMERA_TWO;

	toggle_camera(new, 1);

	return new;
}

void delete_window(Window** window)
{
	free(*window);
	*window = NULL;
}

float adjust_y(Window* window, float y)
{
	return 2.0 * window->cy - y;
}

float get_window_cx(Window* window)
{
	return window->cx;
}

float get_window_cy(Window* window)
{
	return window->cy;
}

float get_window_radius(Window* window)
{
	return window->size;
}


void toggle_camera(Window* window, int camera)
{
	if(camera == 1)
	{
		window->which_camera = 1;
	}
	else if(camera == 2)
	{
		window->which_camera = 2;
		window->horizontal_angle_two = HORIZONTAL_CAMERA_TWO;
		window->vertical_angle_two = VERTICAL_CAMERA_TWO;
	}
	else
	{
		printf("Camera %d not one neither two.\n", camera);
		exit(0);
	}

}


void config_camera_zero(Window* window, float x, float y, float eye, float front_angle)
{
	window->x_zero = x;
	window->y_zero = y;
	window->z_zero = eye;
	window->to_x_zero = x + cos_rad(front_angle);
	window->to_y_zero = y + sin_rad(front_angle);
	window->to_z_zero = eye;
}

void config_camera_one(Window* window, void* character)
{
	get_cam_tip_to(character, &window->x_one, &window->y_one, &window->z_one, &window->to_x_one, &window->to_y_one, &window->to_z_one);
}


void config_camera_two(Window* window, float x, float y, float z, float front_angle, float dist, float dh_angle, float dv_angle)
{
	window->horizontal_angle_two += dh_angle;
	window->vertical_angle_two += dv_angle;

	if(window->horizontal_angle_two > 360.0)
		window->horizontal_angle_two = 360.0;
	else if(window->horizontal_angle_two < 0.0)
		window->horizontal_angle_two = 0.0;

	if(window->vertical_angle_two > 178.0)
		window->vertical_angle_two = 178.0;
	else if(window->vertical_angle_two < 2.0)
		window->vertical_angle_two = 2.0;

	window->x_two = x + sin_rad(window->vertical_angle_two) * cos_rad(front_angle + window->horizontal_angle_two) * dist; // esse dist aqui tbm ..
	window->y_two = y + sin_rad(window->vertical_angle_two) * sin_rad(front_angle + window->horizontal_angle_two) * dist;
	window->z_two = z + cos_rad(window->vertical_angle_two) * dist; // ver esses  multiplicadores depois de justar o 120 para 90 ou menos.. e ampliar a arena..
	window->to_x_two = x;
	window->to_y_two = y;
	window->to_z_two = z;
}

void use_camera_zero(Window* window)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, WINDOW_WIDTH / WINDOW_SMALL_HEIGHT, 1.0, window->size * 3);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_SMALL_HEIGHT);
    gluLookAt(window->x_zero, window->y_zero, window->z_zero, window->to_x_zero, window->to_y_zero, window->to_z_zero, 0,0,1); // o x e y esta trocado ?
}

static void use_camera_one(Window* window)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, WINDOW_WIDTH / WINDOW_HEIGHT, 1.0, window->size * 3);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    gluLookAt(window->x_one, window->y_one, window->z_one, window->to_x_one, window->to_y_one, window->to_z_one, 0,0,1); // o x e y esta trocado ?
}

static void use_camera_two(Window* window)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, WINDOW_WIDTH / WINDOW_HEIGHT, 1.0, window->size * 3);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    gluLookAt(window->x_two, window->y_two, window->z_two, window->to_x_two, window->to_y_two, window->to_z_two, 0,0,1); // o x e y esta trocado ?
}

void use_camera_one_or_two(Window* window)
{
	if(window->which_camera == 1)
	{
		use_camera_one(window);
	}
	else if(window->which_camera == 2)
	{
		use_camera_two(window);
	}
	else
	{
		printf("Camera %d not one neither two.\n", window->which_camera);
		exit(0);
	}
}

int is_camera_one(Window* window)
{
	return window->which_camera == 1;
}

int is_camera_two(Window* window)
{
	return window->which_camera == 2;
}

void set_ortho_zero(Window* window)
{
	unused(window);
	//glOrtho(window->cx - WINDOW_WIDTH, WINDOW_WIDTH + window->cx, window->cy - WINDOW_HEIGHT, window->cy + WINDOW_HEIGHT, -1.0, 1.0);
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_SMALL_HEIGHT, -1.0, 1.0);
}

void set_ortho_one_or_two(Window* window)
{
	unused(window);
	//glOrtho(window->cx - WINDOW_WIDTH, WINDOW_WIDTH + window->cx, window->cy - WINDOW_HEIGHT, window->cy + WINDOW_HEIGHT, -1.0, 1.0);
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1.0, 1.0);
}





