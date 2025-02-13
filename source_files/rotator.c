// Tene Victor-Gabriel, 315CA

#include <stdbool.h>
#include <stdlib.h>

#include "rotator.h"
#include "selector.h"
#include "image_utils.h"
#include "parser.h"
#include "errors.h"

void rotate_image(void ***body, image_header_t *header, area_t *area,
				  int param_count, char params[][ARGUMENTS_DIM])
{
	// checks whether the image is loaded or not
	if (!(*body)) {
		no_image_error();
		return;
	}

	// checks whether the parameters are valid or not
	if (!check_rotate_params(param_count, params)) {
		invalid_command_error();
		return;
	}

	// obtains the angle and checks whether it's valid or not
	int angle = to_number(params[1]);
	if (!check_angle(angle)) {
		unsupported_angle_error();
		return;
	}

	// checks whether the selection area is a square or the whole image
	int selection_height = area->y2 - area->y1;
	int selection_width  = area->x2 - area->x1;
	if (!whole_image_selected(*header, *area) &&
		selection_height != selection_width
	) {
		square_selection_error();
		return;
	}

	// a negative angle signifies the rotation of the matrix to the left
	// a positive angle signifies the rotation of the matrix to the right
	/*
		thus, a negative angle is equivalent to 360 + <negative_angle>,
		which is a positive angle
	*/
	if (angle < 0) {
		angle = MAX_ANGLE + angle;
	}

	// calculates how many rotations to the right should be executed
	int rotation_count = angle / BASE_ANGLE;

	// rotates the selection area of the image body matrix
	rotate_matrix(body, header, area, rotation_count);

	// the selection area was rotated successfully
	rotate_success_message(to_number(params[1]));
}

void rotate_matrix(void ***body, image_header_t *header, area_t *area,
				   int rotation_count)
{
	// no rotation needs to be performed
	if (rotation_count == 0 || rotation_count == 4) {
		return;
	}

	/*
		defines the default data for the image header and selection area of
		the rotation matrix
	*/
	image_header_t new_header = *header;
	area_t new_area = *area;

	/*
		when the whole image is selected, the rotated body might have different
		dimensions if the image is rotated 90 degrees an odd number of times
	*/
	if (whole_image_selected(*header, *area) && rotation_count % 2 != 0) {
		swap_nums(&new_header.width, &new_header.height);
		modify_area(&new_area,
					(area_t){ 0, 0, new_header.width, new_header.height });
	}

	// creates a new matrix where the rotated image is stored
	void **new_body = alloc_matrix(new_header);

	/*
		when the whole image isn't selected, the rotated body shall have the
		same pixels as the initial body except for the pixels inside the
		selection area
	*/
	if (!whole_image_selected(*header, *area)) {
		copy_matrices(&new_body, *body, new_header);
	}

	// the image type/format (PBM, PGM, PPM)
	int type = new_header.type;

	// rotates the selection area based on the <rotation_count>
	for (int i = new_area.y1; i < new_area.y2; i++) {
		for (int j = new_area.x1; j < new_area.x2; j++) {
			rotate_pixel(type, rotation_count, *area, new_area, &new_body,
						 *body, i, j);
		}
	}

	// clear the current image and create the new rotated one
	clear_image(body, header);
	*header = new_header;
	*body = new_body;
	*area = new_area;
}

void rotate_pixel(int type, int rotation_count, area_t old_area,
				  area_t new_area, void ***new_body, void **old_body,
				  int i, int j)
{
	// the initial(old) and after rotation(new) selection area coordinates
	const int old_x1 = old_area.x1;
	const int old_y1 = old_area.y1;
	const int old_x2 = old_area.x2;
	const int old_y2 = old_area.y2;
	const int new_x1 = new_area.x1;
	const int new_y1 = new_area.y1;

	// modifies each pixel of the body to obtain the rotated image
	switch (rotation_count) {
	case 1: {
		if (type == 1) {
			COPY(bwpixel_t, *new_body, i, j, old_body,
				 old_y2 - (j - new_x1) - 1, old_x1 + (i - new_y1));
		} else if (type == 2) {
			COPY(gpixel_t, *new_body, i, j, old_body,
				 old_y2 - (j - new_x1) - 1, old_x1 + (i - new_y1));
		} else {
			COPY(cpixel_t, *new_body, i, j, old_body,
				 old_y2 - (j - new_x1) - 1, old_x1 + (i - new_y1));
		}
		break;
	}
	case 2: {
		if (type == 1) {
			COPY(bwpixel_t, *new_body, i, j, old_body,
				 old_y2 - (i - new_y1) - 1, old_x2 - (j - new_x1) - 1);
		} else if (type == 2) {
			COPY(gpixel_t, *new_body, i, j, old_body,
				 old_y2 - (i - new_y1) - 1, old_x2 - (j - new_x1) - 1);
		} else {
			COPY(cpixel_t, *new_body, i, j, old_body,
				 old_y2 - (i - new_y1) - 1, old_x2 - (j - new_x1) - 1);
		}
		break;
	}
	case 3: {
		if (type == 1) {
			COPY(bwpixel_t, *new_body, i, j, old_body,
				 old_y1 + (j - new_x1), old_x2 - (i - new_y1) - 1);
		} else if (type == 2) {
			COPY(gpixel_t, *new_body, i, j, old_body,
				 old_y1 + (j - new_x1), old_x2 - (i - new_y1) - 1);
		} else {
			COPY(cpixel_t, *new_body, i, j, old_body,
				 old_y1 + (j - new_x1), old_x2 - (i - new_y1) - 1);
		}
		break;
	}
	default:
		break;
	}
}

bool check_angle(int angle)
{
	// checks whether the angle is within the optimal range
	if (angle < MIN_ANGLE || angle > MAX_ANGLE) {
		return false;
	}

	// checks whether the angle is divisible with 90
	if (angle % 90 != 0) {
		return false;
	}

	return true;
}

bool check_rotate_params(int param_count, char params[][ARGUMENTS_DIM])
{
	/*
		the ROTATE command is valid when the input consists of:
		- the command itself
		- the rotation angle (integer)
	*/
	return param_count == 2 && is_number(params[1]);
}
