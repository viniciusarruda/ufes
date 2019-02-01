/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */
 
#include "../util/get_memory.h"
#include "../color/color.h"
#include "window.h"
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>

void init_window(Window* window)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(window->width, window->height); 
	glutInitWindowPosition (window->x, window->y);
	glutCreateWindow(window->title);
	glClearColor(get_red(window->background_color), get_green(window->background_color), get_blue(window->background_color), 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, window->width, 0.0, window->height, -1.0, 1.0);
}

Window* new_window(int width, int height, const char* title, Color* background_color) 
{
	Window* new;
	get_memory(new, sizeof(Window));
	new->width = width;
	new->height = height;
	new->x = 300;
	new->y = 300;
	new->title = title;
	new->background_color = background_color;
	init_window(new);
	return new;
}

void delete_window(Window** window)
{
	free(*window);
	*window = NULL;
}

int adjust_x(Window* window, int x)
{
	return x;
}

int adjust_y(Window* window, int y)
{
	return window->height - y;
}

/* ITS MY CAMERA :)*/ 



