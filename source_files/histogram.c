// Tene Victor-Gabriel, 315CA

#include <stdbool.h>
#include <stdio.h>

#include "histogram.h"
#include "image_utils.h"
#include "errors.h"

void create_histogram(void **body, image_header_t header,
					  int param_count, char params[][ARGUMENTS_DIM])
{
	// checks whether the image is loaded or not
	if (!body) {
		no_image_error();
		return;
	}

	// checks whether the parameters are valid or not
	if (!check_histogram_params(param_count, params)) {
		invalid_command_error();
		return;
	}

	// checks whether the image is grayscale or not
	if (!is_grayscale(header)) {
		grayscale_image_error();
		return;
	}

	// histogram data
	int star_count = to_number(params[1]);
	int bin_count  = to_number(params[2]);

	// checks whether the number of bins is valid or not
	if (bin_count < LOWER_POW_LIM ||
		bin_count > UPPER_POW_LIM ||
		!is_pow2(bin_count)
	) {
		invalid_bins_error();
		return;
	}

	int histogram[MAX_BIN_COUNT] = { 0 };
	compute_histogram(histogram, body, header, star_count, bin_count);
	print_histogram(histogram, bin_count);
}

void compute_histogram(int histogram[], void **body, image_header_t header,
					   int star_count, int bin_count)
{
	// the interval size of the histogram
	const int interval_size = MAX_BIN_COUNT / bin_count;

	// stores the number of stars for each bin
	int stars_freq[MAX_BIN_COUNT] = { 0 };
	int max_freq = 0;

	// the image dimensions
	int height = header.height;
	int width  = header.width;

	// determines the number of stars for each interval of the histogram
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// the value of the pixel at (i, j)
			gpixel_t pixel_value = VALUE(gpixel_t, body, i, j);

			/*
				increases the frequency of the current pixel value for the
				current bin interval, that can be calculated by dividing the
				value of the pixel to the interval size of the histogram
			*/
			stars_freq[pixel_value / interval_size]++;

			// determines the maximum frequency for the bins of the histogram
			int curr_freq = stars_freq[pixel_value / interval_size];
			if (curr_freq > max_freq) {
				max_freq = curr_freq;
			}
		}
	}

	/*
		calculates the histogram with the following properties:
		- has <bin_count> bins
		- has maximum <star_count> stars
	*/
	for (int i = 0; i < bin_count; i++) {
		histogram[i] = (1.0 * stars_freq[i] / max_freq) * star_count;
	}
}

void print_histogram(int histogram[], int bin_count)
{
	for (int i = 0; i < bin_count; i++) {
		printf("%d\t|\t", histogram[i]);
		for (int j = 0; j < histogram[i]; j++) {
			printf("*");
		}
		printf("\n");
	}
}

bool check_histogram_params(int param_count, char params[][ARGUMENTS_DIM])
{
	/*
		the HISTOGRAM command is valid when there are exactly three parameters:
		- the command itself
		- maximum number of stars(*) <=> X
		- number of bins <=> Y
	*/
	return (param_count == 3 && is_number(params[1]) && is_number(params[2]));
}
