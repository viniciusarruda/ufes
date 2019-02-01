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
#include "mouse.h"
#include <GL/glut.h>

#define INITIAL_X 0
#define INITIAL_Y 0

typedef struct
{
	int is_pressed;

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
	int x;
	int y;
	int dx;
	int dy;
	
	int last_motion;
};   

static Button* new_button(void)
{
	Button* new;
	get_memory(new, sizeof(Button));
	new->is_pressed = 0;
	new->mouse_pressed_x = INITIAL_X;
	new->mouse_pressed_y = INITIAL_Y;
	new->mouse_released_x = INITIAL_X;
	new->mouse_released_y = INITIAL_Y;
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
	new->dx = 0;
	new->dy = 0;
	new->last_motion = glutGet(GLUT_ELAPSED_TIME);
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

	free_memory(mouse);
}


static void update_mouse_button(Button* button, int state, int x, int y)
{
	switch(state)
	{
		case GLUT_DOWN:
			button->is_pressed = 1;
			button->mouse_pressed_x = x;
			button->mouse_pressed_y = y;
			break;

		case GLUT_UP:
			button->is_pressed = 0;
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
	mouse->last_motion = glutGet(GLUT_ELAPSED_TIME);
	mouse->dx =  mouse->x - x;
	mouse->dy =  mouse->y - y;
	mouse->x = x;
	mouse->y = y;
}


int get_dx(Mouse* mouse) 
{
	/* 5 miliseconds, is a delay to "identify" if the mouse is in movement */
	if(glutGet(GLUT_ELAPSED_TIME) - mouse->last_motion < 5) 
		return mouse->dx;

	return 0;
}

int get_dy(Mouse* mouse) 
{
	/* 5 miliseconds, is a delay to "identify" if the mouse is in movement */
	if(glutGet(GLUT_ELAPSED_TIME) - mouse->last_motion < 5) 
		return mouse->dy;

	return 0;
}

int get_left_mouse_pressed_x(Mouse* mouse) { return mouse->left->mouse_pressed_x; }
int get_right_mouse_pressed_x(Mouse* mouse) { return mouse->right->mouse_pressed_x; }

int get_left_mouse_pressed_y(Mouse* mouse) { return mouse->left->mouse_pressed_y; }
int get_right_mouse_pressed_y(Mouse* mouse) { return mouse->right->mouse_pressed_y; }

int is_left_mouse_pressed(Mouse* mouse) { return mouse->left->is_pressed; }
int is_right_mouse_pressed(Mouse* mouse) { return mouse->right->is_pressed; }
