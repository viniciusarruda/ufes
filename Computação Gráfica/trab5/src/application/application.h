/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#ifndef APP_H
#define APP_H

#include "../object/arena/arena.h"
#include "../object/character/character.h"

typedef struct Application_t Application;

void run_application(int argc, char** argv);

void set_arena(Application* app, Arena* arena); 
void set_player(Application* app, Character* player); 
void app_add_hurdle(Application* app, float radius, float cx, float cy, float red, float green, float blue);
void app_add_enemy(Application* app, int id, float radius, float cx, float cy, float red, float green, float blue);
void app_add_bullet(Type who_shot_type, int who_shot_id, float cx, float cy, float cz, float ux, float uy, float uz, float bullet_speed);

#endif

