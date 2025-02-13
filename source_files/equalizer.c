// Tene Victor-Gabriel, 315CA

#include <math.h>

#include "equalizer.h"
#include "image_utils.h"
#include "errors.h"

void equalize_image(void ***body, image_header_t header, int param_count)
{
	// checks whether the image is loaded or not
	if (!(*body)) {
		no_image_error();
		return;
	}

	// checks whether the parameters are valid or not
	if (!check_equalize_params(param_count)) {
		invalid_command_error();
		return;
	}

	// checks whether the image is grayscale or not
	if (!is_grayscale(header)) {
		grayscale_image_error();
		return;
	}

	/*
		creates and calculates the histogram that stores the frequency of each
		pixel value found in the image body
	*/
	int histogram[MAX_PIXEL_VALUE + 1] = { 0 };
	calculate_histogram(histogram, *body, header);

	// determines the image area in pixels
	int height = header.height;
	int width = header.width;
	int area = height * width;

	// equalizes each pixel of the image body
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			gpixel_t curr_val = VALUE(gpixel_t, *body, i, j);
			gpixel_t new_val = get_equalized_pixel(curr_val, histogram, area);
			ASSIGN(gpixel_t, *body, i, j, new_val);
		}
	}

	// the image was equalized successfully
	equalize_success_message();
}

void calculate_histogram(int histogram[], void **body, image_header_t header)
{
	// the image dimensions
	int height = header.height;
	int width = header.width;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// the current pixel value
			gpixel_t pixel_value = VALUE(gpixel_t, body, i, j);

			// increase the frequency in the histogram for the current value
			histogram[pixel_value]++;
		}
	}
}

gpixel_t get_equalized_pixel(gpixel_t pixel_value, int histogram[], int area)
{
	/*
		the pixel equalization formula is:
		new_pixel_value = quotient * frequency_sum, where:
		- quotient = 255 / area of the image
		- frequency_sum = sum of the frequencies of all pixel values smaller
		or equal to the current pixel value
	*/

	// the quotient from the formula
	double quotient = 1.0 * MAX_PIXEL_VALUE / area;

	// the frequency_sum from the formula
	int freq_sum = 0;
	for (int i = 0; i <= pixel_value; i++) {
		freq_sum += histogram[i];
	}

	// calculates the new_pixel_value from the formula
	/*
		it also makes use of clamping in order to keep the values between
		0(minimum pixel value) and 255(maximum pixel value)
	*/
	gpixel_t new_pixel_value = round(clamp(quotient * freq_sum));
	return new_pixel_value;
}

bool check_equalize_params(int param_count)
{
	/*
		the EQUALIZE command is valid when there is exactly one parameter:
		- the command itself
	*/
	return param_count == 1;
}
