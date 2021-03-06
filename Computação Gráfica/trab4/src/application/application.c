/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "../window/window.h"
#include "../input/mouse/mouse.h"
#include "../input/keyboard/keyboard.h"

#include "../scoreboard/scoreboard.h"

#include "../object/arena/arena.h"
#include "../shape/circle/circle.h"
#include "../object/hurdles/hurdles.h"
#include "../object/bullets/bullets.h"
#include "../util/math/math.h"

#include "../object/character/character.h"
#include "../object/player/player.h"
#include "../object/enemies/enemies.h"

#include "application_loader/svg_loader.h"
#include "application_loader/config_loader.h"
#include "../util/util.h"
#include "../util/list/list.h"
#include "application.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


struct Application_t
{
    /* Interaction objects */
    Arena* arena;
    Hurdles* hurdles;
    Enemies* enemies;
    Character* player;
    Bullets* bullets;
    /***********************/ 

    /* Inputs */
    Mouse* mouse;
    Keyboard* keyboard;
    /**********/

    Scoreboard* scoreboard;
    Window* window;
};

static Application* app = NULL;

static void build_application(int argc, char** argv)
{
    Config* config;
    config = get_config(argc, argv);

    srand( (unsigned) time(NULL));

    /* Set things with svg file */
    set_app_with_svg(app, get_full_path(config));

    /* Set things with config file, after created with svg */
    set_speed(app->player, get_speed(config), get_shot_speed(config));
    set_enemies_speed(app->enemies, get_enemy_speed(config), get_enemy_shot_speed(config));
    set_percentage(app->hurdles, get_hurdle_percentage(config));

    set_shot_freq(app->enemies, get_freq_shot(config));

    app->bullets = new_bullets();
    
    delete_config(&config);
}

static void adjust_application(void)
{
    adjust_character(app->player, app->window);
    adjust_arena(app->arena, app->window);
    adjust_enemies(app->enemies, app->window);
    adjust_hurdles(app->hurdles, app->window);
}

static void new_application(int argc, char** argv)
{
    get_memory(app, sizeof(Application));

    app->arena = NULL;
    app->hurdles = new_hurdles();
    app->enemies = new_enemies();
    app->player = NULL;
    app->bullets = NULL;

    build_application(argc, argv);
    
    glutInit(&argc, argv);
    app->mouse = new_mouse();
    app->keyboard = new_keyboard();
    app->window = new_window(get_arena_cx(app->arena), get_arena_cy(app->arena), get_arena_size(app->arena), "Vinicius Arruda");
    app->scoreboard = new_scoreboard(number_of_enemies(app->enemies), get_window_cx(app->window), get_window_cy(app->window), get_window_radius(app->window));

    adjust_application();
}

static void delete_application(Application** app)
{
    delete_hurdles(&(*app)->hurdles);
    delete_enemies(&(*app)->enemies);
    delete_bullets(&(*app)->bullets);
    delete_arena((void **) &(*app)->arena);
    delete_character((void **) &(*app)->player);
    delete_window(&(*app)->window);
    delete_scoreboard(&(*app)->scoreboard);
    delete_keyboard(&(*app)->keyboard);
    delete_mouse(&(*app)->mouse);
    free_memory(app);
}

void set_arena(Application* app, Arena* arena) { app->arena = arena; }
void set_player(Application* app, Character* player) { app->player = player; }
void app_add_hurdle(Application* app, float radius, float cx, float cy, float red, float green, float blue) { add_hurdle(app->hurdles, radius, cx, cy, red, green, blue); }
void app_add_enemy(Application* app, int id, float radius, float cx, float cy, float red, float green, float blue) { add_enemy(app->enemies, id, radius, cx, cy, red, green, blue); }
void app_add_bullet(Type who_shot_type, int who_shot_id, float cx, float cy, float orientation, float bullet_speed) { add_bullet(app->bullets, who_shot_type, who_shot_id, cx, cy, orientation, bullet_speed); }


/* APPLICATION INTERACTION */

void display_callback(void)
{  
    glClear(GL_COLOR_BUFFER_BIT);

    /* display must be in the bellow order !!! */
    display_arena(app->arena);
    display_hurdles(app->hurdles);
    display_enemies(app->enemies);
    display_character(app->player);
    display_bullets(app->bullets, app->arena, app->hurdles, app->enemies, app->player, app->scoreboard);
    display_scoreboard(app->scoreboard);

    glutSwapBuffers();
}

void mouse_callback(int button, int state, int x, int y)
{
    y = adjust_y(app->window, y);
    update_mouse(app->mouse, button, state, x, y);

    if(!game_finished(app->scoreboard) && character_shooted(app->player, is_left_mouse_pressed(app->mouse)))
        glutPostRedisplay();
}

void motion_callback(int x, int y) 
{   
    y = adjust_y(app->window, y);
    update_motion(app->mouse, x, y);
    
    if(!game_finished(app->scoreboard) && character_move_gun(app->player, get_dx(app->mouse)))
        glutPostRedisplay();
}

void app_key_pressed(unsigned char key, int x, int y)
{
    unused(x);
    y = adjust_y(app->window, y);
    key_pressed(app->keyboard, key);
    if(!game_finished(app->scoreboard)) character_jump(app->player, is_key_pressed(app->keyboard, 'p'));
}

void app_key_released(unsigned char key, int x, int y)
{
    unused(x);
    y = adjust_y(app->window, y);
    key_released(app->keyboard, key);
}

void idle(void)
{
    int dirty = 0;
    int up_down = 0;
    int left_right = 0;

    if(has_bullets(app->bullets))
        dirty = 1;

    if(!game_finished(app->scoreboard))
    {
        if(is_key_pressed(app->keyboard, 'w') && !is_key_pressed(app->keyboard, 's'))
            up_down = 1;
        else if(!is_key_pressed(app->keyboard, 'w') && is_key_pressed(app->keyboard, 's'))
            up_down = -1;

        if(is_key_pressed(app->keyboard, 'a') && !is_key_pressed(app->keyboard, 'd'))
            left_right = 1;
        else if(!is_key_pressed(app->keyboard, 'a') && is_key_pressed(app->keyboard, 'd'))
            left_right = -1;

        character_jump(app->player, is_key_pressed(app->keyboard, 'p'));

        if(idle_character(app->player, app->arena, app->hurdles, app->enemies, app->player, up_down, left_right, player_collided, app->enemies))
            dirty = 1;

        if(enemies_behave(app->enemies, app->player, app->arena, app->hurdles))
            dirty = 1;
    }

    if(dirty)
        glutPostRedisplay();
}


void run_application(int argc, char** argv)
{
    new_application(argc, argv);

    glutDisplayFunc(display_callback); 
    glutMouseFunc(mouse_callback);
    glutMotionFunc(motion_callback);
    glutPassiveMotionFunc(motion_callback);

    glutKeyboardFunc(app_key_pressed);
    glutKeyboardUpFunc(app_key_released);
    glutIdleFunc(idle);

    glutMainLoop(); 

    // HOW DO I FREE THE ALLOCATED MEMORY ?

    delete_application(&app);
}

