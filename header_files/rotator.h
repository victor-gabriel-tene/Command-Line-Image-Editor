// Tene Victor-Gabriel, 315CA

#pragma once

#include <stdbool.h>
#include "parser.h"
#include "image_utils.h"

#define MIN_ANGLE -360
#define MAX_ANGLE 360
#define BASE_ANGLE 90

// rotates the whole image or the selection area of the image
void rotate_image(void ***body, image_header_t *header, area_t *area,
				  int param_count, char params[][ARGUMENTS_DIM]);

// check the validity of the rotation angle
/*
	the angle is considered valid if it's between -360 and 360 and it's
	divisible with 90
*/
bool check_angle(int angle);

// rotates the selection area of the image to the right <rotation_count> times
void rotate_matrix(void ***body, image_header_t *header, area_t *area,
				   int rotation_count);

// moves the current pixel (i, j) at the correct position in the rotated image
void rotate_pixel(int type, int rotation_count, area_t old_area,
				  area_t new_area, void ***new_body, void **old_body,
				  int i, int j);

// checks the validity of the parameters of the ROTATE command
bool check_rotate_params(int param_count, char params[][ARGUMENTS_DIM]);
