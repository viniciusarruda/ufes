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
#include "window.h"
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
};


void init_window(Window* window)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(window->size * 2, window->size * 2); 
	glutInitWindowPosition(300, 300);
	glutCreateWindow(window->title);
	glClearColor(MAX_COLOR, MAX_COLOR, MAX_COLOR, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(window->cx - window->size, window->size + window->cx, window->cy - window->size, window->cy + window->size, -1.0, 1.0);
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
