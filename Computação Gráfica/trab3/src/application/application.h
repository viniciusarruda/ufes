/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of shooting and improvements on moving.
 *
 */
 
#ifndef APP_H
#define APP_H

#include "../object/arena/arena.h"
#include "../object/player/player.h"
#include "../object/hurdle/hurdle.h"

typedef struct Application_t Application;

void run_application(int argc, char** argv);

void set_arena(Application* app, Arena* arena); 
void set_player(Application* app, Player* player); 
void append_small_hurdle(Application* app, Hurdle* hurdle);
void append_big_hurdle(Application* app, Hurdle* hurdle);
void app_add_bullet(float cx, float cy, float orientation);

#endif

