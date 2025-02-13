// Tene Victor-Gabriel, 315CA

#pragma once

#include <stdbool.h>
#include "image_utils.h"
#include "parser.h"

#define MAX_BIN_COUNT 256

// the number of bins should range within these limits
#define LOWER_POW_LIM 2
#define UPPER_POW_LIM 256

// calculates the histogram and prints it to the screen
void create_histogram(void **body, image_header_t header,
					  int param_count, char params[][ARGUMENTS_DIM]);

// computes the histogram by calculating the number of stars for each bin
void compute_histogram(int histogram[], void **body, image_header_t header,
					   int star_count, int bin_count);

// prints the histogram
void print_histogram(int histogram[], int bin_count);

// checks the validity of the parameters of the HISTOGRAM command
bool check_histogram_params(int param_count, char params[][ARGUMENTS_DIM]);
