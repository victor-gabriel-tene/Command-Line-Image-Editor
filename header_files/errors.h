// Tene Victor-Gabriel, 315CA

#pragma once

#include "parser.h"

// error messages

// prints an error message when receiving an invalid command
void invalid_command_error(void);

// prints an error message when the image fails to load
void load_failure_error(const char *image_name);

// prints an error message when the image saving process fails
void save_failure_error(const char *image_name);

// prints an error moessage when the image cropping process fails
void crop_failure_error(void);

/*
	prints an error message when the provided coordinates are not within the
	optimal range determined by the image header
*/
void invalid_coordinates_error(void);

// prints an error message when the number of bins of a histogram is not valid
// the number of bins in a histogram should be a power of 2 between 2 and 256
void invalid_bins_error(void);

// prints an error message when the image isn't grayscale (PGM format)
void grayscale_image_error(void);

// prints an error message when the rotation angle isn't valid
void unsupported_angle_error(void);

// prints an error message if the selection isn't a square or the whole image
void square_selection_error(void);

// prints an error message if the filter isn't valid
void invalid_filter_error(void);

// prints an error message if the image isn't color
void color_image_error(void);

// prints an error message if there is no image loaded
void no_image_error(void);

// success messages

// prints a success message when the image is loaded
void load_success_message(const char *image_name);

// prints a success message when the image is saved
void save_success_message(const char *image_name);

// prints a message when the image selection succeeds
void select_success_message(int param_count, char params[][ARGUMENTS_DIM]);

// prints a success message after the image gets cropped
void crop_success_message(void);

// prints a success message after the image gets equalized
void equalize_success_message(void);

// prints a success message after the selection area gets rotated
void rotate_success_message(int angle);

// prints a success message after the filter was applied successfully
void apply_success_message(char *filter);
