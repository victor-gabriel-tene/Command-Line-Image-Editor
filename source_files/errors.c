// Tene Victor-Gabriel, 315CA

#include <stdio.h>

#include "errors.h"

void invalid_command_error(void)
{
	printf("Invalid command\n");
}

void load_failure_error(const char *image_name)
{
	printf("Failed to load %s\n", image_name);
}

void save_failure_error(const char *image_name)
{
	printf("Failed to save %s\n", image_name);
}

void crop_failure_error(void)
{
	printf("Failed to crop the image\n");
}

void invalid_coordinates_error(void)
{
	printf("Invalid set of coordinates\n");
}

void invalid_bins_error(void)
{
	printf("Invalid number of bins\n");
}

void grayscale_image_error(void)
{
	printf("Black and white image needed\n");
}

void unsupported_angle_error(void)
{
	printf("Unsupported rotation angle\n");
}

void square_selection_error(void)
{
	printf("The selection must be square\n");
}

void invalid_filter_error(void)
{
	printf("APPLY parameter invalid\n");
}

void color_image_error(void)
{
	printf("Easy, Charlie Chaplin\n");
}

void no_image_error(void)
{
	printf("No image loaded\n");
}

void load_success_message(const char *image_name)
{
	printf("Loaded %s\n", image_name);
}

void save_success_message(const char *image_name)
{
	printf("Saved %s\n", image_name);
}

void select_success_message(int param_count, char params[][ARGUMENTS_DIM])
{
	printf("Selected ");
	for (int i = 1; i < param_count; i++) {
		printf("%s ", params[i]);
	}
	printf("\n");
}

void crop_success_message(void)
{
	printf("Image cropped\n");
}

void equalize_success_message(void)
{
	printf("Equalize done\n");
}

void rotate_success_message(int angle)
{
	printf("Rotated %d\n", angle);
}

void apply_success_message(char *filter)
{
	printf("APPLY %s done\n", filter);
}
