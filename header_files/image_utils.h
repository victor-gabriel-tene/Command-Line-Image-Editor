// Tene Victor-Gabriel, 315CA

#pragma once

// libraries

#include <stdbool.h>
#include "parser.h"

// type definitions

typedef unsigned char byte_t;

// the image header
typedef struct {
	/*
		represents the image type:
		P1 -> black and white text image
		P2 -> grayscale text image
		P3 -> color text image
		P4 -> black and white binary image
		P5 -> grayscale binary image
		P6 -> color binary image
	*/
	char magic_word[3];

	/*
		type = 1 <=> P1/P4 (black and white image) <=> PBM format
		type = 2 <=> P2/P5 (grayscale image) <=> PGM format
		type = 3 <=> P3/P6 (color image) <=> PPM format
	*/
	int type;

	// the width and height of the image
	int width, height;

	// the maximum value that each pixel channel can hold (255 in this project)
	int max_value;
} image_header_t;

// the black and white pixel
typedef bool bwpixel_t;

// the grayscale pixel
typedef unsigned char gpixel_t;

// the color pixel
typedef struct {
	byte_t red, green, blue; // RGB channels
} cpixel_t;

// the selected area coordinates
typedef struct {
	int x1, y1; // the coordinates of the upper-left corner of the area
	int x2, y2; // the coordinates of the lower-right corner of the area
} area_t;

// macros

#define COMMAND_DIM 10
#define IMAGE_NAME_DIM 100
#define MIN_PIXEL_VALUE 0
#define MAX_PIXEL_VALUE 255

// image body value macro
#define VALUE(type, body, i, j) \
	(*((type *)((body)[(i)]) + (j)))

// image body assign macro
#define ASSIGN(type, body, i, j, val) \
	(*((type *)((body)[(i)]) + (j)) = (val))

// image body copy macro
#define COPY(type, body1, i1, j1, body2, i2, j2) \
	(*((type *)((body1)[(i1)]) + (j1)) = \
	*((type *)((body2)[(i2)]) + (j2)))

// functions

// checks if an image is grayscale (PGM format)
bool is_grayscale(image_header_t header);

// checks if an image is color (PPM format)
bool is_color(image_header_t header);

// limits <value> in the following range: <MIN_PIXEL_VALUE>...<MAX_PIXEL_VALUE>
double clamp(double value);

// allocates the memory for the image body described inside the <header>
void **alloc_matrix(image_header_t header);

// resizes the matrix <body> to the new size provided by the header
// the previous matrix values are lost, thus a copy is needed
bool resize_matrix(void ***body, image_header_t old_header,
				   image_header_t new_header);

/*
	copies each element from one matrix to the other taking their types into
	account, which are stored in the image header
*/
void copy_matrices(void ***body1, void **body2, image_header_t header);

// clears the image header
void clear_header(image_header_t *header);

// clears the image body
void clear_body(void ***body, image_header_t header);

// clears both the image header and the image body
void clear_image(void ***body, image_header_t *header);

// checks whether a string is a number or not
bool is_number(const char *string);

// checks whether a number is a power of 2
bool is_pow2(int num);

// transforms a string in its corresponding number
int to_number(const char *string);

// swaps two integers
void swap_nums(int *a, int *b);

// swaps two strings
void swap_strings(char *s1, char *s2);
