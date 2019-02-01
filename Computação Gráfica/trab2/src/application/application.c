/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a player and hurdles with collision detection and jump effect.
 *
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "../window/window.h"
#include "../input/mouse/mouse.h"
#include "../input/keyboard/keyboard.h"

#include "../object/arena/arena.h"
#include "../object/player/player.h"
#include "../object/circle/circle.h"
#include "../object/hurdle/hurdle.h"
#include "../util/math/math.h"

#include "application_loader/svg_loader.h"
#include "application_loader/path_loader.h"
#include "../util/util.h"
#include "../util/list/list.h"
#include "application.h"

#include <stdio.h>
#include <stdlib.h>


struct Application_t
{
    /* Interaction objects */
    Arena* arena;
    List* small_hurdles;
    List* big_hurdles;
    Player* player;
    /***********************/ 

    /* Inputs */
    Mouse* mouse;
    Keyboard* keyboard;
    /**********/

    Window* window;
};

static Application* app = NULL;

static void build_application(int argc, char** argv)
{
    Path* path;
    path = get_path(argc, argv);
    set_app_with_svg(app, get_full_path(path));
    delete_path(&path);
}

static void adjust_application(void)
{
    adjust_player(app->player, app->window);
    adjust_arena(app->arena, app->window);
    traverse_arg(app->small_hurdles, adjust_hurdle, app->window);
    traverse_arg(app->big_hurdles, adjust_hurdle, app->window);
}

static void new_application(int argc, char** argv)
{
    get_memory(app, sizeof(Application));

    app->arena = NULL;
    app->small_hurdles = new_list();
    app->big_hurdles = new_list();
    app->player = NULL;

    build_application(argc, argv);
    
    glutInit(&argc, argv);
    app->mouse = new_mouse();
    app->keyboard = new_keyboard();
    app->window = new_window(get_arena_cx(app->arena), get_arena_cy(app->arena), get_arena_size(app->arena), "Vinicius Arruda");

    adjust_application();
}

static void delete_application(Application** app)
{
    delete_list(&(*app)->small_hurdles, delete_hurdle);
   	delete_list(&(*app)->big_hurdles, delete_hurdle);
    delete_arena((void **) &(*app)->arena);
    delete_player((void **) &(*app)->player);
    delete_window(&(*app)->window);
    delete_keyboard(&(*app)->keyboard);
    delete_mouse(&(*app)->mouse);
    free_memory(app);
}

void set_arena(Application* app, Arena* arena) { app->arena = arena; }
void set_player(Application* app, Player* player) { app->player = player; }
void append_small_hurdle(Application* app, Hurdle* hurdle) { append(app->small_hurdles, hurdle); }
void append_big_hurdle(Application* app, Hurdle* hurdle) { append(app->big_hurdles, hurdle); }


/* APPLICATION INTERACTION */


void display_callback(void)
{  
    glClear(GL_COLOR_BUFFER_BIT);

    display_arena(app->arena);
    traverse(app->small_hurdles, display_hurdle);
    traverse(app->big_hurdles, display_hurdle);
    display_player(app->player);

    glutSwapBuffers();
}

void mouse_callback(int button, int state, int x, int y)
{
    y = adjust_y(app->window, y);
    update_mouse(app->mouse, button, state, x, y);
}


void motion_callback(int x, int y) 
{   
    y = adjust_y(app->window, y);
    update_motion(app->mouse, x, y);
}

void app_key_pressed(unsigned char key, int x, int y)
{
    unused(x);
    y = adjust_y(app->window, y);
    key_pressed(app->keyboard, key);
    jump_player(app->player, app->keyboard);
}


void app_key_released(unsigned char key, int x, int y)
{
    unused(x);
    y = adjust_y(app->window, y);
    key_released(app->keyboard, key);
}

void idle(void)
{
    if(idle_player(app->player, app->arena, app->small_hurdles, app->big_hurdles, app->mouse, app->keyboard)) 
        glutPostRedisplay();
}


void run_application(int argc, char** argv)
{
    new_application(argc, argv);

    glutDisplayFunc(display_callback); 
    // glutMouseFunc(mouse_callback);
    // glutMotionFunc(motion_callback);

    glutKeyboardFunc(app_key_pressed);
    glutKeyboardUpFunc(app_key_released);
    glutIdleFunc(idle);

    glutMainLoop();

    // HOW DO I FREE THE ALLOCATED MEMORY ?

    delete_application(&app);
}

