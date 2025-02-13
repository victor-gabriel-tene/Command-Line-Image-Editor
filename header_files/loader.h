// Tene Victor-Gabriel, 315CA

#pragma once

#include <stdbool.h>
#include "image_utils.h"
#include "parser.h"

#define LINE_LENGTH 1000

// loads both the header and the body of the image
void load_image(void ***body, image_header_t *header, area_t *area,
				int param_count, char params[][ARGUMENTS_DIM]);

// loads the header of the image named: <image_name>
// returns the position of the file cursor or -1 if any errors occured
long load_header(const char *image_name, image_header_t *header);

// loads the body of the image named: <image_name> and returns a pointer to it
void **load_body(const char *image_name, image_header_t header, long cursor);

// clears a line from <file> if it contains a comment
void clear_comment(FILE *file);

// checks the validity of the parameters of the LOAD command
bool check_load_params(int param_count);
