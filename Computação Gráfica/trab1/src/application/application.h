/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */
 
#ifndef APP_H
#define APP_H

#define CAST_APP(a) ((Application *) (a))

typedef enum
{
    APP,
    WINDOW,
    WIDTH,
    HEIGHT,
    BACKGROUND,
    TITLE,
    CIRCLE
} Tag;

typedef struct
{
    Tag current_tag;
    int width;
    int height;
    double bg_r;
    double bg_g;
    double bg_b;
    char* title;
    double radius;
    double circle_r;
    double circle_g;
    double circle_b;
} Application;

Application* new_application(void);
void delete_application(Application** app);

#endif

