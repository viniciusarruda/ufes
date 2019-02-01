/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */
 
#ifndef COLOR_H
#define COLOR_H

typedef struct Color_t Color;

Color* new_color(double r, double g, double b);
void delete_color(Color** color);

void set_red(Color* color, double r);
void set_green(Color* color, double g);
void set_blue(Color* color, double b);

double get_red(Color* color);
double get_green(Color* color);
double get_blue(Color* color);

#endif
