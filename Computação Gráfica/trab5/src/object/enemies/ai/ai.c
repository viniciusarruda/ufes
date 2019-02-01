/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 

#include "ai.h"
#include <time.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "../../character/character.h"

#define TL 0
#define TR 1
#define FW 2
#define FJ 3
#define TX 4
#define FX 5
#define RX 6
#define FP 7

int table[4][4][4] = 
{
	{
		{ FX, TL, TR, TR },
		{ TX, TL, TR, TR },
		{ FW, FW, TR, TR },
		{ FW, FW, FW, TR }
	},

	{
		{ TR, RX, TR, TR },
		{ TR, RX, TR, TR },
		{ FW, FW, TR, TR },
		{ FW, FW, FW, TR }
	},

	{
		{ TL, TL, FW, TR },
		{ TL, TL, RX, TR },
		{ TL, TL, FJ, TR },
		{ FW, FW, FW, TR }
	},

	{
		{ TL, TL, TL, FW },
		{ TL, TL, TL, RX },
		{ TL, TL, TL, FW },
		{ TL, TL, TL, FP }
	}
};


void ai_output(Character* character, float freq_shot, int* up_down, int* left_right, int* jump, int* shoot)
{
	int character_consume_movement = get_consume_movement(character);
	int op = table[get_sensor_0(character)][get_sensor_1(character)][get_sensor_2(character)];


	if(freq_shot != 0.0 && glutGet(GLUT_ELAPSED_TIME) - get_dt_shoot(character) > (1000.0 / freq_shot))
	{
		reset_dt_shoot(character);
		*shoot = 1;
	}
	else
	{
		*shoot = 0;
	}


	if(glutGet(GLUT_ELAPSED_TIME) - get_dt_space(character) > 1000)
	{
		reset_dt_space(character);

		if(!get_rotate_state(character) && in_center_space(character) && op != FP)  
		{
			toggle_rotate_state(character);
		}

		set_center_space(character);
	}


	if(get_rotate_state(character))
	{
		*left_right = get_swap_movement(character);
		*up_down = 0;  
		*jump = 0;

		set_consume_movement(character, character_consume_movement - 1);

		if(character_consume_movement == 0)
		{
			set_consume_movement(character, (int) rand() % 100); 
			toggle_rotate_state(character);
		}

		return;
	}


	if(character_consume_movement == 0)
	{
		set_consume_movement(character, (int) rand() % 100);
		swap_movement(character);
	}
	else
	{
		set_consume_movement(character, character_consume_movement - 1);
	}

	switch(op)
	{
		case TR:
			*left_right = -1;
			*up_down = 1;
			*jump = 0;
			break;

		case TL:
			*left_right = 1;
			*up_down = 1;
			*jump = 0;
			break;

		case FP:
		case FW:
			*left_right = 0;
			*up_down = 1;
			*jump = 0;
			break;

		case FJ:
			*left_right =0;
			*up_down = 1;
			*jump = 1;
			break;

		case TX:
			*left_right = get_swap_movement(character);
			*up_down = -1;  
			*jump = 0;
			break;

		case FX:
			*left_right = get_swap_movement(character);
			*up_down = 1;  
			*jump = 0;
			break;

		case RX:
			*left_right = get_swap_movement(character);
			*up_down = 0; 
			*jump = 0;
			set_consume_movement(character, (int) rand() % 100); 
			toggle_rotate_state(character);
			break;
	}
}




