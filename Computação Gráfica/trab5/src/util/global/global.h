/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <GL/glut.h>

#define ANGLE_COXA 20.0
#define ANGLE_CANELA 50.0

#define HIGH_DENSITY_SLICES 100
#define HIGH_DENSITY_LOOPS  100
#define LOW_DENSITY_SLICES 20 
#define LOW_DENSITY_LOOPS 20

#define WINDOW_WIDTH 500.0
#define WINDOW_HEIGHT 500.0
#define WINDOW_SMALL_HEIGHT 200.0
#define WINDOW_TOP_HEIGHT 700.0

#define HORIZONTAL_CAMERA_TWO 180.0
#define VERTICAL_CAMERA_TWO 45.0


/* Multipliers */
#define HEAD 0.25
#define HEAD_HEIGHT (HEAD * 1.8)
#define NECK_HEIGHT (HEAD * 0.5)

#define ARM 1.75
#define ARMR_TOP 0.125
#define ARMR_MID 0.1
#define ARMR_LOW 0.06

#define LEG_PART 1.0
#define LEGR 0.25
#define LEGR_LOW (LEGR * 0.4)
#define LEGR_MID (LEGR * 0.65)
#define LEGR_TOP LEGR
#define BODY_HEIGHT_TOP 0.2
#define BODY_HEIGHT_MID 0.7
#define BODY_HEIGHT_LOW 0.3
#define BODY_WIDTH_TOP (LEGR_TOP * 2.0)
#define BODY_WIDTH_BASE (LEGR_TOP * 2.0)
#define BODY_WIDTH_MID (LEGR_TOP * 1.7)
#define BODY_THICK 0.5

#define BODY_THIN_SCALE (LEGR * 3.0)
#define BODY_WIDTH_SCALE (LEGR * 5.0)

#define FULL_BODY_HEIGHT (BODY_HEIGHT_LOW + BODY_HEIGHT_MID + BODY_HEIGHT_TOP)

#define CHARACTER_HEIGHT (LEG_PART + LEG_PART + FULL_BODY_HEIGHT + NECK_HEIGHT + HEAD_HEIGHT)

#define EYE_HEIGHT (CHARACTER_HEIGHT - HEAD_HEIGHT * 0.25)

#define ARENA_HEIGHT 4.0

typedef enum 
{
	NONE_mat,
	SHININESS_mat,
	LOW_mat,
	HARD_LOW_mat,
	HIGH_mat,
	FULL_mat
} Mat_type;

GLfloat* global_mat(Mat_type type);


#endif
