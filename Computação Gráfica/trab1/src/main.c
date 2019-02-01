/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "application/application.h"
#include "xml_parser/xml_application_parser.h"
#include "window/window.h"
#include "color/color.h"
#include "input/mouse/mouse.h"
#include "shape/circle/circle.h"
#include "shape/line/line.h"
#include "util/error.h"
#include "util/get_memory.h"

#include <string.h>

#define DEFAULT_XML_CONFIG_FILE "config.xml"
#define CIRCLE_MIN_RADIUS 10.0

typedef enum {STATE_CREATION, STATE_DRAG_OR_RESIZE, STATE_DRAG, STATE_RESIZE} State;

void state_drag(int x, int y);
void state_resize(int x, int y);

Window* window = NULL;
Mouse* mouse = NULL;
Circle* circle = NULL;
Color* circle_color = NULL;
Color* background_color = NULL;
double circle_radius;

State program_state = STATE_CREATION;
void (*state_callback) (int, int) = NULL;

void display_callback(void)
{  
	glClear(GL_COLOR_BUFFER_BIT);

	if(program_state != STATE_CREATION)
		circle->display(circle);

	glutSwapBuffers();
}

void mouse_callback(int button, int state, int x, int y)
{
	x = adjust_x(window, x);
	y = adjust_y(window, y);

	update_mouse(mouse, button, state, x, y);

	switch(button)
	{
		case GLUT_LEFT_BUTTON:

			if(program_state == STATE_CREATION && state == GLUT_DOWN) 
			{
				circle = new_circle(circle_radius, CIRCLE_MIN_RADIUS, x, y, circle_color);
				glutPostRedisplay();
				program_state = STATE_DRAG_OR_RESIZE;
				state_callback = NULL;
			}		
			else if(state == GLUT_UP && program_state == STATE_DRAG)
			{
				program_state = STATE_DRAG_OR_RESIZE;
				state_callback = NULL;
			}
			else if(state == GLUT_DOWN && program_state == STATE_DRAG_OR_RESIZE)
			{
				if(point_in_circle(circle, get_left_mouse_pressed_x(mouse), get_left_mouse_pressed_y(mouse)))
				{
					program_state = STATE_DRAG;
					state_callback = state_drag;
				}
			}
			break;

		case GLUT_RIGHT_BUTTON:   

			if(state == GLUT_UP && program_state == STATE_RESIZE)
			{
				program_state = STATE_DRAG_OR_RESIZE;
				state_callback = NULL;
			}
			else if(state == GLUT_DOWN && program_state == STATE_DRAG_OR_RESIZE)
			{
				if(point_in_circle(circle, get_right_mouse_pressed_x(mouse), get_right_mouse_pressed_y(mouse)))
				{
					program_state = STATE_RESIZE;
					state_callback = state_resize;
				}
			}
			break;
	}
	
}


void state_drag(int x, int y)
{
	set_center_x(circle, get_center_x(circle) + x - get_last_x(mouse));
	set_center_y(circle, get_center_y(circle) + y - get_last_y(mouse));
	glutPostRedisplay();
}

void state_resize(int x, int y)
{
	double tmp1, tmp2, tmp3, project_x, project_y;

	if(x == get_center_x(circle) && y == get_center_y(circle)) return; /* avoid zero division */

	project_s_onto_l(get_last_x(mouse) - get_center_x(circle), get_last_y(mouse) - get_center_y(circle), x - get_center_x(circle), y - get_center_y(circle), &project_x, &project_y);
	tmp1 = pythagorean_c(x - get_center_x(circle), y - get_center_y(circle));
	tmp2 = pythagorean_c(get_last_x(mouse) - get_center_x(circle), get_last_y(mouse) - get_center_y(circle));
	tmp3 = pythagorean_c(x - get_center_x(circle) - project_x, y - get_center_y(circle) - project_y);
	set_radius(circle, get_radius(circle) + ((tmp1 > tmp2) ? tmp3 : - tmp3));
	
	glutPostRedisplay();
}

void motion_callback(int x, int y)  /* always when some button is pressed !!!! */
{	
	x = adjust_x(window, x);
	y = adjust_y(window, y);

	update_motion(mouse, x, y);

	if(state_callback != NULL)
		state_callback(x, y);
}


int main(int argc, char** argv)
{
	Application* app;
	char* file_path;

	if(argc > 1)
	{
		get_memory(file_path, sizeof(char) * (strlen(DEFAULT_XML_CONFIG_FILE) + strlen(argv[1]) + 1));
		file_path[0] = '\0';
		strcat(file_path, argv[1]);
		strcat(file_path, DEFAULT_XML_CONFIG_FILE);
		app = get_xml_content(file_path);
		free(file_path);
		file_path = NULL;
	}
	else
	{
		app = get_xml_content(DEFAULT_XML_CONFIG_FILE);
	}

	glutInit(&argc, argv);

	circle_radius = app->radius;
	background_color = new_color(app->bg_r, app->bg_g, app->bg_b);
	window = new_window(app->width, app->height, app->title, background_color);
	mouse = new_mouse();
	circle_color = new_color(app->circle_r, app->circle_g, app->circle_b);

	glutDisplayFunc(display_callback); 
	glutMouseFunc(mouse_callback);
	glutMotionFunc(motion_callback);

	glutMainLoop();

	// delete_application(&app);

	// HOW DO I FREE THE ALLOCATED MEMORY ?

	return 0;
}

