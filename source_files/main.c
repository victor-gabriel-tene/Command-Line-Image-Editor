// Tene Victor-Gabriel, 315CA

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "loader.h"
#include "saver.h"
#include "selector.h"
#include "cropper.h"
#include "histogram.h"
#include "equalizer.h"
#include "rotator.h"
#include "applier.h"
#include "quit.h"
#include "errors.h"
#include "image_utils.h"
#include "parser.h"

int main(void)
{
	int param_count = 0;
	char params[ARGUMENTS_CNT][ARGUMENTS_DIM];

	image_header_t header = { "", 0, 0, 0, 0 };
	area_t area = { 0, 0, 0, 0 };
	void **body = NULL;

	do {
		// parses the input which consists of the command and its parameters
		param_count = parse_input(params);
		// quits the program when no more parameters are found
		if (param_count == 0) {
			break;
		}

		// obtaining the command from the parameter list
		char command[COMMAND_DIM];
		strcpy(command, params[0]);

		// executes the actions corresponding to that specific command
		if (strcmp(command, "LOAD") == 0) {
			load_image(&body, &header, &area, param_count, params);
		} else if (strcmp(command, "SELECT") == 0) {
			select_area(&area, body, header,  param_count, params);
		} else if (strcmp(command, "ROTATE") == 0) {
			rotate_image(&body, &header, &area, param_count, params);
		} else if (strcmp(command, "EQUALIZE") == 0) {
			equalize_image(&body, header, param_count);
		} else if (strcmp(command, "CROP") == 0) {
			crop_image(&body, &header, &area, param_count);
		} else if (strcmp(command, "APPLY") == 0) {
			apply_filter(&body, header, area, param_count, params);
		} else if (strcmp(command, "SAVE") == 0) {
			save_image(body, header, param_count, params);
		} else if (strcmp(command, "HISTOGRAM") == 0) {
			create_histogram(body, header, param_count, params);
		} else if (strcmp(command, "EXIT") == 0) {
			if (quit(&body, &header, param_count)) {
				break;
			}
		} else {
			invalid_command_error();
		}
	} while (true);

	return EXIT_SUCCESS;
}
