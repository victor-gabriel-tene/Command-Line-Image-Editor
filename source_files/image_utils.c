// Tene Victor-Gabriel, 315CA

#include <stdlib.h>
#include <string.h>

#include "image_utils.h"
#include "parser.h"
#include "errors.h"

bool is_grayscale(image_header_t header)
{
	return header.type == 2;
}

bool is_color(image_header_t header)
{
	return header.type == 3;
}

double clamp(double value)
{
	if (value < MIN_PIXEL_VALUE) {
		return MIN_PIXEL_VALUE;
	}
	if (value > MAX_PIXEL_VALUE) {
		return MAX_PIXEL_VALUE;
	}
	return value;
}

void **alloc_matrix(image_header_t header)
{
	// the image data
	int height = header.height;
	int width = header.width;
	int type = header.type;

	// sets the size of the current pixel determined by the image type
	size_t pixel_size = 0;
	if (type == 1) {
		pixel_size = sizeof(bwpixel_t);
	} else if (type == 2) {
		pixel_size = sizeof(gpixel_t);
	} else {
		pixel_size = sizeof(cpixel_t);
	}

	// allocates the array of pointers
	void **mat = (void **)malloc(height * sizeof(void *));
	if (!mat) {
		return NULL;
	}

	for (int i = 0; i < height; i++) {
		// allocates the necessary memory to storing each row of elements
		mat[i] = malloc(width * pixel_size);

		// frees the already allocated rows if the current row allocation fails
		if (!mat[i]) {
			for (int j = 0; j < i; j++) {
				free(mat[j]);
			}
			free(mat);
			return NULL;
		}
	}

	return mat;
}

bool resize_matrix(void ***body, image_header_t old_header,
				   image_header_t new_header)
{
	// deallocates the memory used for the image body
	clear_body(body, old_header);

	// allocates new memory in order to satisfy the new header data
	*body = alloc_matrix(new_header);

	// returns true if resizing succeeded and false otherwise
	return ((*body) != NULL);
}

void copy_matrices(void ***body1, void **body2, image_header_t header)
{
	// the image data
	int height = header.height;
	int width = header.width;
	int type = header.type;

	// copies each element from one matrix to the other
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (type == 1) {
				COPY(bwpixel_t, *body1, i, j, body2, i, j);
			} else if (type == 2) {
				COPY(gpixel_t, *body1, i, j, body2, i, j);
			} else {
				COPY(cpixel_t, *body1, i, j, body2, i, j);
			}
		}
	}
}

void clear_header(image_header_t *header)
{
	// reinitializes the members of the image header
	strcpy(header->magic_word, "");
	header->type = 0;
	header->width = 0;
	header->height = 0;
	header->max_value = 0;
}

void clear_body(void ***body, image_header_t header)
{
	// checks whether the image is loaded or not
	if (!(*body)) {
		return;
	}

	// frees each row of the matrix
	for (int i = 0; i < header.height; i++) {
		free((*body)[i]);
	}

	// frees the array of pointers and sets the main pointer to NULL
	free(*body);
	*body = NULL;
}

void clear_image(void ***body, image_header_t *header)
{
	clear_body(body, *header);
	clear_header(header);
}

bool is_number(const char *string)
{
	// checks for negative numbers
	// obtains the starting point of the digits
	int start = 0;
	if (string[0] == '-') {
		start = 1;
	}

	// checks whether all characters are digits or not
	for (int i = start; string[i] != '\0'; i++) {
		if (string[i] < '0' || string[i] > '9') {
			return false;
		}
	}
	return true;
}

bool is_pow2(int num)
{
	if (num == 0) {
		return false;
	}

	while (num % 2 == 0) {
		num /= 2;
	}
	return num == 1;
}

int to_number(const char *string)
{
	int number = 0;
	int len = strlen(string);

	// obtains the number sign and the starting point of the digits
	int sign = 1, start = 0;
	if (string[0] == '-') {
		sign = -1;
		start = 1;
	}

	// calculates the number
	for (int i = start; i < len; i++) {
		number = number * 10 + (string[i] - '0');
	}

	return number * sign;
}

void swap_nums(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void swap_strings(char *s1, char *s2)
{
	char s[BUFFER_SIZE];
	strcpy(s, s1);
	strcpy(s1, s2);
	strcpy(s2, s);
}
