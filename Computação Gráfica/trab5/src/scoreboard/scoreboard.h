/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

typedef struct Scoreboard_t Scoreboard;

Scoreboard* new_scoreboard(int number_of_enemies, float cx, float cy, float radius);
void delete_scoreboard(Scoreboard** scoreboard);
void display_scoreboard(Scoreboard* scoreboard);
void enemy_killed(Scoreboard* scoreboard);
void player_killed(Scoreboard* scoreboard);
int game_finished(Scoreboard* scoreboard);
void display_minimap(Scoreboard* scoreboard, void* arena, void* player, void* enemies, void* hurdles);

#endif
