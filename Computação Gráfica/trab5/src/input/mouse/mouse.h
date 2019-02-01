/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
 
#ifndef MOUSE_H
#define MOUSE_H

typedef struct Mouse_t Mouse;

Mouse* new_mouse(void);
void delete_mouse(Mouse** mouse);
void update_mouse(Mouse* mouse, int button, int state, int x, int y);
void update_motion(Mouse* mouse, int x, int y);

int get_dx(Mouse* mouse);
int get_dy(Mouse* mouse);

int get_left_mouse_pressed_x(Mouse* mouse);
int get_right_mouse_pressed_x(Mouse* mouse);

int get_left_mouse_pressed_y(Mouse* mouse);
int get_right_mouse_pressed_y(Mouse* mouse);

int is_left_mouse_pressed(Mouse* mouse);
int is_right_mouse_pressed(Mouse* mouse);

#endif
