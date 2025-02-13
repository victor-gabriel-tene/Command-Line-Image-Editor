// Tene Victor-Gabriel, 315CA

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "loader.h"
#include "selector.h"
#include "image_utils.h"
#include "errors.h"

void load_image(void ***body, image_header_t *header, area_t *area,
				int param_count, char params[][ARGUMENTS_DIM])
{
	/*
		clears the image header and the image body in case an image
		was already loaded
	*/
	clear_image(body, header);

	// check whether the parameters are valid or not
	if (!check_load_params(param_count)) {
		invalid_command_error();
		return;
	}

	// obtains the name of the image file
	char image_name[IMAGE_NAME_DIM];
	strcpy(image_name, params[1]);

	// tries to load the header and gets the position of the file cursor
	long file_cursor = load_header(image_name, header);
	if (file_cursor == -1) {
		return;
	}

	// tries to load the image body
	// if it fails, the image header gets cleared
	*body = load_body(image_name, *header, file_cursor + 1);
	if (!(*body)) {
		clear_header(header);
		return;
	}

	// selects the whole image
	const area_t whole_image_selection = {
		0, 0,
		header->width, header->height
	};
	modify_area(area, whole_image_selection);

	load_success_message(image_name);
}

long load_header(const char *image_name, image_header_t *header)
{
	// tries to open the file named <image_name>
	FILE *image_file = fopen(image_name, "rt");
	if (!image_file) {
		load_failure_error(image_name);
		return -1;
	}

	// reads the image magic word
	clear_comment(image_file);
	fscanf(image_file, "%s", header->magic_word);

	// sets the type of the image
	header->type = (header->magic_word[1] - '0') % 3;
	if (header->type == 0) {
		header->type = 3;
	}

	// reads the image width and height
	clear_comment(image_file);
	fscanf(image_file, "%d%d", &header->width, &header->height);

	/*
		reads the maximum value of the pixel channels
		the pixel value of black and white images(in PBM format) can be either
		0 or 1, thus the maximum value of a pixel channel is set to 1
	*/
	if (header->type > 1) {
		clear_comment(image_file);
		fscanf(image_file, "%d", &header->max_value);
	} else {
		header->max_value = 1;
	}
	clear_comment(image_file);

	// save the position of the file cursor and closes the file afterwards
	long pos = ftell(image_file);
	fclose(image_file);
	return pos;
}

void **load_body(const char *image_name, image_header_t header, long cursor)
{
	// allocates the memory for the image body
	void **body = alloc_matrix(header);
	if (!body) {
		return NULL;
	}

	// determines whether the image is in ascii or binary format
	int p_value = header.magic_word[1] - '0';
	const char *mode = (p_value <= 3 ? "rt" : "rb");

	// tries to open the file named <image_name> in the proper mode
	FILE *image_file = fopen(image_name, mode);
	if (!image_file) {
		load_failure_error(image_name);
		return NULL;
	}
	// moves the cursor to the position where the header data ended
	fseek(image_file, cursor, SEEK_SET);

	// the image data
	int height = header.height;
	int width = header.width;

	if (p_value <= 3) { // PBM, PGM and PPM formats in ASCII
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				byte_t pixel_value;
				byte_t red, green, blue;

				if (p_value <= 2) {
					// reads the pixel channel
					fscanf(image_file, "%hhu", &pixel_value);
				} else {
					// reads the RGB pixel channels
					fscanf(image_file, "%hhu%hhu%hhu", &red, &green, &blue);
				}

				// casts the image body to the correct pixel type
				if (p_value == 1) {
					// assigns the pixel value (0 / 1) to the current element
					ASSIGN(bwpixel_t, body, i, j, pixel_value);
				} else if (p_value == 2) {
					// assigns the pixel value (0 - 255) to the current element
					ASSIGN(gpixel_t, body, i, j, pixel_value);
				} else {
					/*
						assigns each RGB value (0 - 255) to its corresponding
						pixel channel of the current element
					*/
					ASSIGN(cpixel_t, body, i, j,
						   ((cpixel_t){ red, green, blue }));
				}
			}
		}
	} else { // PBM, PGM and PPM formats in binary
		for (int i = 0; i < height; i++) {
			/*
				reads the bytes for a row of pixels of a specific type and then
				stores the data in the current row of the image body
			*/
			if (p_value == 4) {
				fread((bwpixel_t *)(body[i]), sizeof(bwpixel_t), width,
					  image_file);
			} else if (p_value == 5) {
				fread((gpixel_t *)(body[i]), sizeof(gpixel_t), width,
					  image_file);
			} else {
				fread((cpixel_t *)(body[i]), sizeof(cpixel_t), width,
				      image_file);
			}
		}
	}

	// closes the image file
	fclose(image_file);
	return body;
}

void clear_comment(FILE *file)
{
	// initializes the line
	char line[LINE_LENGTH];
	memset(line, 0, sizeof(line));

	// reads the whole line (assuming that a line has maximum 1000 characters)
	fgets(line, LINE_LENGTH, file);

	// the length of the previously read line
	int line_length = strlen(line);

	/*
		if the current line didn't contain a comment, the file cursor is moved
		to its initial position
	*/
	if (line[0] != '#') {
		fseek(file, -line_length, SEEK_CUR);
	}
}

bool check_load_params(int param_count)
{
	/*
		the LOAD command is valid when there are exactly two parameters:
		- the command itself
		- the file name
	*/
	return param_count == 2;
}
