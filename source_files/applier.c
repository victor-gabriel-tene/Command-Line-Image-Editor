// Tene Victor-Gabriel, 315CA

#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "applier.h"
#include "image_utils.h"
#include "parser.h"
#include "errors.h"

void apply_filter(void ***body, image_header_t header, area_t area,
				  int param_count, char params[][ARGUMENTS_DIM])
{
	// checks whether the image is loaded or not
	if (!(*body)) {
		no_image_error();
		return;
	}

	// checks whether the parameters are valid or not
	if (!check_apply_params(param_count)) {
		invalid_command_error();
		return;
	}
	if (!check_filter(params[1])) {
		invalid_filter_error();
		return;
	}

	// checks whether the image is color or not
	if (!is_color(header)) {
		color_image_error();
		return;
	}

	// the image kernels used for applying the filter to the pixels
	const double kernels[KERNEL_COUNT][MATRIX_DIM][MATRIX_DIM] = {
		EDGE_KERNEL, SHARPEN_KERNEL, BOX_BLUR_KERNEL, GAUSSIAN_BLUR_KERNEL
	};

	/*
		determines the kernel type:
		- type 0 <=> EDGE kernel
		- type 1 <=> SHARPEN kernel
		- type 2 <=> BOX BLUR kernel
		- type 3 <=> GAUSSIAN BLUR kernel
	*/
	int kernel_type;
	if (strcmp(params[1], "EDGE") == 0) {
		kernel_type = 0;
	} else if (strcmp(params[1], "SHARPEN") == 0) {
		kernel_type = 1;
	} else if (strcmp(params[1], "BLUR") == 0) {
		kernel_type = 2;
	} else {
		kernel_type = 3;
	}

	// create a copy that will store the image after applying the filter
	image_header_t new_header = header;
	void **new_body = alloc_matrix(new_header);
	copy_matrices(&new_body, *body, new_header);

	// overwrites the selection area with the new pixel values
	for (int i = area.y1; i < area.y2; i++) {
		for (int j = area.x1; j < area.x2; j++) {
			if (has_all_neighbours(header, i, j)) {
				cpixel_t new_pixel = calc_pixel_sum(*body, i, j,
													kernels[kernel_type]);
				ASSIGN(cpixel_t, new_body, i, j, new_pixel);
			}
		}
	}

	// update the current image and clear the copy
	copy_matrices(body, new_body, header);
	clear_image(&new_body, &new_header);

	// the filter was applied successfully to the selection area
	apply_success_message(params[1]);
}

bool has_all_neighbours(image_header_t header, int i, int j)
{
	// the image dimensions
	int height = header.height;
	int width = header.width;

	return i > 0 && i < height - 1 && j > 0 && j < width - 1;
}

cpixel_t calc_pixel_sum(void **body, int i, int j,
						const double kernel[MATRIX_DIM][MATRIX_DIM])
{
	// direction arrays
	// identify all neighbours of the (i, j) pixel
	const int dir_i[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	const int dir_j[] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

	// the sum for each pixel channel
	double red_sum = 0;
	double green_sum = 0;
	double blue_sum = 0;

	// the current direction
	int dir = 0;

	// calculates the sum for each pixel channel accordingly
	for (int k_i = 0; k_i < MATRIX_DIM; k_i++) {
		for (int k_j = 0; k_j < MATRIX_DIM; k_j++) {
			// the coordinates of the neighbour
			int new_i = i + dir_i[dir];
			int new_j = j + dir_j[dir];

			// updating the sum for each neighbour
			red_sum += VALUE(cpixel_t, body, new_i, new_j).red *
					   kernel[k_i][k_j];
			green_sum += VALUE(cpixel_t, body, new_i, new_j).green *
						 kernel[k_i][k_j];
			blue_sum += VALUE(cpixel_t, body, new_i, new_j).blue *
						kernel[k_i][k_j];

			dir++;
		}
	}

	// the updated pixel (after applying the filter)
	cpixel_t pixel = {
		round(clamp(red_sum)),
		round(clamp(green_sum)),
		round(clamp(blue_sum))
	};

	return pixel;
}

bool check_filter(char *filter)
{
	// checks each possible filter and returns the validitiy of the parameter
	bool is_edge = strcmp(filter, "EDGE") == 0;
	bool is_sharpen = strcmp(filter, "SHARPEN") == 0;
	bool is_blur = strcmp(filter, "BLUR") == 0;
	bool is_gaussian_blur = strcmp(filter, "GAUSSIAN_BLUR") == 0;

	return is_edge || is_sharpen || is_blur || is_gaussian_blur;
}

bool check_apply_params(int param_count)
{
	/*
		the APPLY command is valid when exactly two parameters exist:
		- the command itself
		- the apply parameter
	*/

	return param_count == 2;
}
