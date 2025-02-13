// Tene Victor-Gabriel, 315CA

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "selector.h"
#include "image_utils.h"
#include "errors.h"

void select_area(area_t *area, void **body, image_header_t header,
				 int param_count, char params[][ARGUMENTS_DIM])
{
	// checks whether the parameters are valid or not
	if (!check_select_params(param_count, params)) {
		invalid_command_error();
		return;
	}

	// checks whether the image is loaded or not
	if (!body) {
		no_image_error();
		return;
	}

	// represents the new area that shall be selected
	area_t new_area;
	if (strcmp(params[1], "ALL") == 0) {
		// the new area represents the whole image in this case
		new_area = (area_t) {
			0, 0, header.width, header.height
		};
	} else {
		// the new area is represented by the 4 provided command parameters
		new_area = (area_t) {
			to_number(params[1]),
			to_number(params[2]),
			to_number(params[3]),
			to_number(params[4]),
		};

		// checks whether the coordinates are within the provided range
		if (!check_coordinates(new_area, header)) {
			invalid_coordinates_error();
			return;
		}

		/*
			swaps the x and y coordinates so that:
			x1 < x2 and y1 < y2
		*/
		if (new_area.x1 > new_area.x2) {
			swap_nums(&new_area.x1, &new_area.x2);
			swap_strings(params[1], params[3]);
		}
		if (new_area.y1 > new_area.y2) {
			swap_nums(&new_area.y1, &new_area.y2);
			swap_strings(params[2], params[4]);
		}
	}

	// modifies the current area to the new area
	modify_area(area, new_area);

	// prints a success message since no errors occured
	select_success_message(param_count, params);
}

void modify_area(area_t *area, area_t new_area)
{
	*area = new_area;
}

bool whole_image_selected(image_header_t header, area_t area)
{
	const int height = header.height;
	const int width  = header.width;

	return area.x1 == 0 && area.y1 == 0 &&
		   area.x2 == width && area.y2 == height;
}

bool check_coordinates(area_t area, image_header_t header)
{
	/*
		the optimal coordinates ranges:
		x coordinates <--> x_start...x_end
		y coordinates <--> y_start...y_end
	*/
	const int x_start = 0;
	const int y_start = 0;
	const int x_end = header.width;
	const int y_end = header.height;

	// checks if the coordinates point to a non-empty area
	if (area.x1 == area.x2 || area.y1 == area.y2) {
		return false;
	}

	// checks if the x1 coordinate is within optimal range
	if (area.x1 < x_start || area.x1 > x_end) {
		return false;
	}

	// checks if the x2 coordinate is within optimal range
	if (area.x2 < x_start || area.x2 > x_end) {
		return false;
	}

	// checks if the y1 coordinate is within optimal range
	if (area.y1 < y_start || area.y1 > y_end) {
		return false;
	}

	// checks if the y2 coordinate is within optimal range
	if (area.y2 < y_start || area.y2 > y_end) {
		return false;
	}

	return true;
}

bool check_select_params(int param_count, char params[][ARGUMENTS_DIM])
{
	/*
		the SELECT command is valid when the input consists of:
		1) the command itself and 4 coordinates (integers)
		2) the command itself and the flag "ALL"
	*/
	return ((param_count == 5 &&
			 is_number(params[1]) && is_number(params[2]) &&
			 is_number(params[3]) && is_number(params[4])) ||
			 (param_count == 2 && strcmp(params[1], "ALL") == 0));
}
