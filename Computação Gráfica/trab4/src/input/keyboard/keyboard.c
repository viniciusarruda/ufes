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
#include "keyboard.h"
#include <string.h>
#include <stdio.h>

#define RELEASED 0
#define PRESSED  1

struct Keyboard_t
{
	int key[256];
};

Keyboard* new_keyboard(void)
{
	Keyboard* new;
	get_memory(new, sizeof(Keyboard));
	memset(new->key, RELEASED, 256 * sizeof(int));
	return new; 
}

void delete_keyboard(Keyboard** keyboard)
{
	free_memory(keyboard);
}

void key_pressed(Keyboard* keyboard, unsigned char key)
{
	keyboard->key[key] = PRESSED;
}

void key_released(Keyboard* keyboard, unsigned char key)
{
	keyboard->key[key] = RELEASED;
}

int is_key_pressed(Keyboard* keyboard, unsigned char key)
{
	return keyboard->key[key] == PRESSED;
}

