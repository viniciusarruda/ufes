/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */
 
#ifndef WINDOW_H
#define WINDOW_H

#include "../color/color.h"

typedef struct
{
	int width;
	int height;
	int x;
	int y;
	const char* title;
	Color* background_color;
} Window;

Window* new_window(int width, int height, const char* title, Color* background_color);
void delete_window(Window** window);
int adjust_x(Window* window, int x);
int adjust_y(Window* window, int y);

#endif
