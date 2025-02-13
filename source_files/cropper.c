// Tene Victor-Gabriel, 315CA

#include "cropper.h"
#include "selector.h"
#include "image_utils.h"
#include "errors.h"

void crop_image(void ***body, image_header_t *header, area_t *area,
				int param_count)
{
	// checks whether the image is loaded or not
	if (!(*body)) {
		no_image_error();
		return;
	}

	// checks whether the parameters are valid or not
	if (!check_crop_params(param_count)) {
		invalid_command_error();
		return;
	}

	// making a copy of the initial image body matrix
	image_header_t old_header = *header;
	void **old_body = alloc_matrix(old_header);
	copy_matrices(&old_body, *body, old_header);

	// creating a new header that has the new dimensions for the cropped image
	image_header_t new_header = *header;
	new_header.width  = area->x2 - area->x1;
	new_header.height = area->y2 - area->y1;

	// tries to resize the matrix to the dimensions store in <new_header>
	if (!resize_matrix(body, old_header, new_header)) {
		crop_failure_error();
		return;
	}

	/*
		copies the pixels of the selected submatrix from the inital image
		to the newly resized image matrix
	*/
	*header = new_header;
	int type = header->type;

	for (int i = 0, sel_i = area->y1; i < header->height; i++, sel_i++) {
		for (int j = 0, sel_j = area->x1; j < header->width; j++, sel_j++) {
			if (type == 1) {
				COPY(bwpixel_t, *body, i, j, old_body, sel_i, sel_j);
			} else if (type == 2) {
				COPY(gpixel_t, *body, i, j, old_body, sel_i, sel_j);
			} else {
				COPY(cpixel_t, *body, i, j, old_body, sel_i, sel_j);
			}
		}
	}

	// resets the current area to the whole cropped image
	area_t new_area = { 0, 0, header->width, header->height };
	modify_area(area, new_area);

	// clears the image copy
	clear_image(&old_body, &old_header);

	// the image was cropped successfully
	crop_success_message();
}

bool check_crop_params(int param_count)
{
	/*
		the CROP command is valid when there is exactly one parameter:
		- the command itself
	*/
	return param_count == 1;
}
