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

/* camera */
void use_camera_zero(Window* window);
void use_camera_one_or_two(Window* window);

int is_camera_one(Window* window);
int is_camera_two(Window* window);

void set_ortho_zero(Window* window);
void set_ortho_one_or_two(Window* window);
void toggle_camera(Window* window, int camera);

void config_camera_zero(Window* window, float x, float y, float eye, float front_angle);
void config_camera_two(Window* window, float x, float y, float z, float front_angle, float dist, float dh_angle, float dv_angle);

void config_camera_one(Window* window, void* character);

#endif
