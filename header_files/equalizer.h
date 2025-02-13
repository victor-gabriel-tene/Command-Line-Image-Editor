// Tene Victor-Gabriel, 315CA

#pragma once

#include <stdbool.h>
#include "image_utils.h"

// equalizes the black and white (PBM and PGM formats) image
void equalize_image(void ***body, image_header_t header, int param_count);

// computes the pixel frequency histogram
void calculate_histogram(int histogram[], void **body, image_header_t header);

// computes the new pixel value after the equalization process
gpixel_t get_equalized_pixel(gpixel_t pixel_value, int histogram[], int area);

// checks the validity of the parameters of the EQUALIZE command
bool check_equalize_params(int param_count);
