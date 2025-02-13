// Tene Victor-Gabriel, 315CA

#pragma once

#include <stdbool.h>
#include "image_utils.h"
#include "parser.h"

#define SELECT_PARAM_DIM 100

// selects the provided area of the image
void select_area(area_t *area, void **body, image_header_t header,
				 int param_count, char params[][ARGUMENTS_DIM]);

// modifies the current selection area of the image with the new one
void modify_area(area_t *area, const area_t new_area);

// checks whether the whole image is selected
bool whole_image_selected(image_header_t header, area_t area);

// checks if the coordinates are within the ranges provided by the image header
bool check_coordinates(area_t area, image_header_t header);

// checks the validity of the parameters of the SELECT command
bool check_select_params(int param_count, char params[][ARGUMENTS_DIM]);
