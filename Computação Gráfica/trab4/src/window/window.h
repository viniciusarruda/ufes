/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */
 
#ifndef WINDOW_H
#define WINDOW_H

typedef struct Window_t Window;

Window* new_window(float cx, float cy, float size, const char* title);
void delete_window(Window** window);
float adjust_y(Window* window, float y);
float get_window_cx(Window* window);
float get_window_cy(Window* window);
float get_window_radius(Window* window);

#endif
