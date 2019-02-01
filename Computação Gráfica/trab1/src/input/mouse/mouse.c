/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */
 
#include "../../util/get_memory.h"
#include "../../util/error.h"
#include "mouse.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>

#define INITIAL_X 0
#define INITIAL_Y 0

typedef struct
{
	/* past situations */

	/* mouse past */
	int mouse_pressed_x;
	int mouse_pressed_y;

	int mouse_released_x;
	int mouse_released_y;
	/***************/
} Button;

struct Mouse_t
{
	/* Possible buttons (needs to implement scroll wheel) */
	Button* left;
	/* Button* middle; */
	Button* right;
	int last_x;
	int last_y;
	int x;
	int y;
};   

static Button* new_button(void)
{
	Button* new;
	get_memory(new, sizeof(Button));
	return new;
}


Mouse* new_mouse(void)
{
	Mouse* new;
	get_memory(new, sizeof(Mouse));
	new->left = new_button();
	/* new->middle = new_button(); */
	new->right = new_button();
	new->x = INITIAL_X;
	new->y = INITIAL_Y;
	return new; 
}

void delete_mouse(Mouse** mouse)
{
	free((*mouse)->left);
	/* free((*mouse)->middle); */
	free((*mouse)->right);

	(*mouse)->left = NULL;
	/* (*mouse)->middle = NULL; */
	(*mouse)->right = NULL;

	free(*mouse);
	*mouse = NULL;
}


static void update_mouse_button(Button* button, int state, int x, int y)
{
	switch(state)
	{
		case GLUT_DOWN:
			button->mouse_pressed_x = x;
			button->mouse_pressed_y = y;
			break;

		case GLUT_UP:
			button->mouse_released_x = x;
			button->mouse_released_y = y;
			break;
	}
}


void update_mouse(Mouse* mouse, int button, int state, int x, int y)
{
	mouse->x = x;
	mouse->y = y;

	switch(button)
	{
		case GLUT_LEFT_BUTTON:
			update_mouse_button(mouse->left, state, x, y);
			break;

		/*case GLUT_MIDDLE_BUTTON:
			update_mouse_button(mouse->middle, state, x, y);
			break;*/

		case GLUT_RIGHT_BUTTON:
			update_mouse_button(mouse->right, state, x, y);    
			break;
	}
}

void update_motion(Mouse* mouse, int x, int y)
{
	mouse->last_x = mouse->x;
	mouse->last_y = mouse->y;
	mouse->x = x;
	mouse->y = y;
}


int get_last_x(Mouse* mouse) { return mouse->last_x; }
int get_last_y(Mouse* mouse) { return mouse->last_y; }

int get_left_mouse_pressed_x(Mouse* mouse) { return mouse->left->mouse_pressed_x; }
int get_right_mouse_pressed_x(Mouse* mouse) { return mouse->right->mouse_pressed_x; }

int get_left_mouse_pressed_y(Mouse* mouse) { return mouse->left->mouse_pressed_y; }
int get_right_mouse_pressed_y(Mouse* mouse) { return mouse->right->mouse_pressed_y; }

