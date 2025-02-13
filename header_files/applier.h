// Tene Victor-Gabriel, 315CA

#pragma once

#include <stdbool.h>

#include "image_utils.h"
#include "parser.h"

#define DIRECTION_COUNT 9
#define KERNEL_COUNT 4
#define MATRIX_DIM 3

// the image kernels
#define EDGE_KERNEL		\
	{					\
		{ -1, -1, -1 }, \
		{ -1, 8, -1 },	\
		{ -1, -1, -1 }	\
	}

#define SHARPEN_KERNEL		\
	{						\
		{ 0, -1, 0 },		\
		{ -1, 5, -1 },		\
		{ 0, -1, 0 }		\
	}

#define BOX_BLUR_KERNEL					\
	{									\
		{ 1.0 / 9, 1.0 / 9, 1.0 / 9 },	\
		{ 1.0 / 9, 1.0 / 9, 1.0 / 9 },	\
		{ 1.0 / 9, 1.0 / 9, 1.0 / 9 }	\
	}

#define GAUSSIAN_BLUR_KERNEL				\
	{										\
		{ 1.0 / 16, 2.0 / 16, 1.0 / 16 },	\
		{ 2.0 / 16, 4.0 / 16, 2.0 / 16 },	\
		{ 1.0 / 16, 2.0 / 16, 1.0 / 16 }	\
	}

// applies a specific filter to the selection area of the image
void apply_filter(void ***body, image_header_t header, area_t area,
				  int param_count, char params[][ARGUMENTS_DIM]);

// checks whether the filter is valid or not
bool check_filter(char *filter);

// checks whether the pixel (i, j) has all neighbours inside the matrix
bool has_all_neighbours(image_header_t header, int i, int j);

// calculates the new value of a pixel after applying the <kernel>

cpixel_t calc_pixel_sum(void **body, int i, int j,
						const double kernel[MATRIX_DIM][MATRIX_DIM]);

// checks the validity of the parameters of the APPLY command
bool check_apply_params(int param_count);
