/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */

// mudar o nome desse arquivo .. pq da o display no minimap tbm...

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "../util/util.h"
#include "../util/global/global.h"
#include "scoreboard.h"
#include "../shape/rectangle/rectangle.h"
#include "../object/arena/arena.h"
#include "../object/character/character.h"
#include "../object/enemies/enemies.h"
#include <unistd.h>

#define SCOREBOARD_FONT GLUT_BITMAP_TIMES_ROMAN_24
#define FONT_HEIGHT 17

#define MSG_ENEMIES_WON ((const unsigned char *) "YOU LOSE !")
#define MSG_PLAYER_WON ((const unsigned char *) "YOU WON !")

#define PLAYER_WON 2
#define ENEMIES_WON 3

struct Scoreboard_t
{
	int length_enemies_won;
	int length_player_won;

	int last_blink;
	int turn;

	int live_enemies;
	int dead_enemies;

	int finished;

	float x2;
	float y2;
	float cx;
	float cy;
	float radius;
};


Scoreboard* new_scoreboard(int number_of_enemies, float cx, float cy, float radius)
{
	Scoreboard* new;
	get_memory(new, sizeof(Scoreboard));

	new->length_enemies_won = glutBitmapLength(SCOREBOARD_FONT, MSG_ENEMIES_WON);
	new->length_player_won = glutBitmapLength(SCOREBOARD_FONT, MSG_PLAYER_WON);

	new->last_blink = glutGet(GLUT_ELAPSED_TIME);
	new->turn = 1;

	new->live_enemies = number_of_enemies;
	new->dead_enemies = 0;
	new->finished = 0;

	new->x2 = cx + radius;
	new->y2 = cy + radius;
	new->cx = cx;
	new->cy = cy;
	new->radius = radius;

	return new;
}

void delete_scoreboard(Scoreboard** scoreboard)
{
	free_memory(scoreboard);
}


int game_finished(Scoreboard* scoreboard)
{
	return scoreboard->finished;
}


static void flush_str(const unsigned char* str, float r, float g, float b)
{
	glColor3f(r, g, b);
	glRasterPos2f(0, 0);

	while(*str)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str++); 
}


static void display_finish_message(const unsigned char* msg, int length, int turn, float cx, float cy)
{
	unused(cx);
	unused(cy);

	glPushMatrix();

		//glTranslatef(WINDOW_WIDTH * 0.5 - length * 0.5, WINDOW_TOP_HEIGHT * 0.5 - FONT_HEIGHT * 0.5, 0.0);
		//glTranslatef(WINDOW_WIDTH - 15 - length, WINDOW_SMALL_HEIGHT - 15 - FONT_HEIGHT, 0.0); 
		glTranslatef(WINDOW_WIDTH * 0.5 - length * 0.5, 15, 0.0); 

		glPushMatrix();
			glTranslatef(-15, -15, 0.0);
			display_corner_L_rectangle(length + 30, FONT_HEIGHT + 30, 1, turn == 1, turn != 1);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-10, -10, 0.0); 
			display_corner_L_rectangle(length + 20, FONT_HEIGHT + 20, 1, turn != 1, turn == 1);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-5, -5, 0.0); 
			display_corner_L_rectangle(length + 10, FONT_HEIGHT + 10, 0, 0, 0);
		glPopMatrix();

		flush_str(msg, 1, turn == 1, turn != 1);

	glPopMatrix(); 
}


static void display_score(const unsigned char* msg, int length, float x2, float y2)
{
	unused(x2);
	unused(y2);

	glPushMatrix();

		glTranslatef(WINDOW_WIDTH - 15 - length, WINDOW_SMALL_HEIGHT - 15 - FONT_HEIGHT, 0.0); 

		glPushMatrix();
			glTranslatef(-15, -15, 0.0);
			display_corner_L_rectangle(length + 30, FONT_HEIGHT + 30, 1, 0, 1);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-10, -10, 0.0); 
			display_corner_L_rectangle(length + 20, FONT_HEIGHT + 20, 1, 1, 0);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-5, -5, 0.0); 
			display_corner_L_rectangle(length + 10, FONT_HEIGHT + 10, 0, 0, 0);
		glPopMatrix();

		flush_str(msg, 1, 0, 1);

	glPopMatrix(); 
}


void display_scoreboard(Scoreboard* scoreboard)
{
	char str[100];

	glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);

		if(scoreboard->finished)
		{
			if(glutGet(GLUT_ELAPSED_TIME) - scoreboard->last_blink > 200)
			{
				scoreboard->last_blink = glutGet(GLUT_ELAPSED_TIME);	
				scoreboard->turn *= -1; 
			}

			if(scoreboard->finished == PLAYER_WON)
				display_finish_message(MSG_PLAYER_WON, scoreboard->length_player_won, scoreboard->turn, scoreboard->cx, scoreboard->cy);
			else
				display_finish_message(MSG_ENEMIES_WON, scoreboard->length_enemies_won, scoreboard->turn, scoreboard->cx, scoreboard->cy);
		}
		else
		{
			snprintf(str, 100, "%d", scoreboard->dead_enemies);
			display_score((const unsigned char *) str, glutBitmapLength(SCOREBOARD_FONT, (const unsigned char *) str), scoreboard->x2, scoreboard->y2);
		}

	glPopAttrib();
}


void display_minimap(Scoreboard* scoreboard, void* arena, void* player, void* enemies, void* hurdles)
{
	glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);

        glPushMatrix();
			
			glTranslatef(WINDOW_WIDTH * 0.75, 0.0, 0.0);

			//display_corner_L_rectangle(WINDOW_WIDTH * 0.25, WINDOW_HEIGHT * 0.25, 1, 1, 0);

			glPushMatrix();
				glLoadIdentity();

				// gl ortho tem que ficar de acordo com a orientação de como está..
				// depois fazer essa conta aqui
				//glTranslatef(250 * (WINDOW_WIDTH * 0.25) / 500.0, 250 * (WINDOW_HEIGHT * 0.25) / 500.0, 0);

				glTranslatef(WINDOW_WIDTH * 0.75, 0.0, 0.0);

				glTranslatef(WINDOW_WIDTH * 0.25 * 0.5, WINDOW_HEIGHT * 0.25 * 0.5, 0);
				glScalef((WINDOW_WIDTH * 0.25) / (scoreboard->radius * 2.0), (WINDOW_WIDTH * 0.25) / (scoreboard->radius * 2.0), 1.0);
				glTranslatef(-scoreboard->cx, -scoreboard->cy, 0);


				display_mini_arena(arena);
				display_mini_hurdles(hurdles);
				display_mini_character(player);
				display_mini_enemies(enemies);

				// printf("(%f, %f)\n", scoreboard->cx, scoreboard->cy);
				// glTranslatef(500, 500, 0);
				// display_corner_L_rectangle(100, 100, 1, 0, 0);


			glPopMatrix();

		glPopMatrix();

	glPopAttrib();
}


void enemy_killed(Scoreboard* scoreboard)
{
	scoreboard->live_enemies--;
	scoreboard->dead_enemies++;

	if(scoreboard->live_enemies == 0)
	{
		scoreboard->finished = PLAYER_WON;
	}
}


void player_killed(Scoreboard* scoreboard)
{
	scoreboard->finished = ENEMIES_WON;
}
