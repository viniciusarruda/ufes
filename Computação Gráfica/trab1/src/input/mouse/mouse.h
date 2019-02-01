/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */
 
 
#ifndef MOUSE_H
#define MOUSE_H

typedef struct Mouse_t Mouse;

Mouse* new_mouse(void);
void delete_mouse(Mouse** mouse);
void update_mouse(Mouse* mouse, int button, int state, int x, int y);
void update_motion(Mouse* mouse, int x, int y);

int get_last_x(Mouse* mouse);
int get_last_y(Mouse* mouse);

int get_left_mouse_pressed_x(Mouse* mouse);
int get_right_mouse_pressed_x(Mouse* mouse);

int get_left_mouse_pressed_y(Mouse* mouse);
int get_right_mouse_pressed_y(Mouse* mouse);

#endif
