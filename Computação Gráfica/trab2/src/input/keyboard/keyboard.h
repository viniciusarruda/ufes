/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a player and hurdles with collision detection and jump effect.
 *
 */
 
 
#ifndef KEYBOARD_H
#define KEYBOARD_H

typedef struct Keyboard_t Keyboard;

Keyboard* new_keyboard(void);
void delete_keyboard(Keyboard** keyboard);
void key_pressed(Keyboard* keyboard, unsigned char key);
void key_released(Keyboard* keyboard, unsigned char key);
int is_key_pressed(Keyboard* keyboard, unsigned char key);

#endif
