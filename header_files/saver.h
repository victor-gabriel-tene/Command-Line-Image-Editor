// Tene Victor-Gabriel, 315CA

#pragma once

#include <stdbool.h>
#include "image_utils.h"
#include "parser.h"

#define ASCII_DIM 6

// the type that defines the parameters of the SAVE command
typedef struct {
	char image_name[IMAGE_NAME_DIM];	// the name of the file
	bool mode;							// the format mode: ASCII/binary
} parameter_t;

// saves both the header and the body of the image
void save_image(void **body, image_header_t header,
				int param_count, char params[][ARGUMENTS_DIM]);

// saves the header of the image named <image_name> in ASCII format
bool save_header(const char *image_name, bool mode, image_header_t header);

/*
	saves the body of the image name <image_name> in ASCII/binary format:
	mode = true  <=> ASCII format
	mode = false <=> binary format
*/
bool save_body(const char *image_name, bool mode,
			   void **body, image_header_t header);

// checks the validity of the parameters of the SAVE command
bool check_save_params(int param_count, char params[][ARGUMENTS_DIM]);
