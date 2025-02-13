// Tene Victor-Gabriel, 315CA

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "saver.h"
#include "errors.h"
#include "image_utils.h"

void save_image(void **body, image_header_t header,
				int param_count, char params[][ARGUMENTS_DIM])
{
	// checks whether the parameters are valid or not
	if (!check_save_params(param_count, params)) {
		invalid_command_error();
		return;
	}

	// checks whether the image is loaded or not
	if (!body) {
		no_image_error();
		return;
	}

	// determines the image name and the ascii flag
	char image_name[IMAGE_NAME_DIM];
	strcpy(image_name, params[1]);
	bool ascii_mode = (param_count == 3);

	if (save_header(image_name, ascii_mode, header) &&
		save_body(image_name, ascii_mode, body, header)) {
		save_success_message(image_name);	// the image was saved successfully
	} else {
		save_failure_error(image_name);		// the image saving process failed
	}
}

bool save_header(const char *image_name, bool mode, image_header_t header)
{
	// tries to open the file named <image_name> in text(ASCII) format
	FILE *image_file = fopen(image_name, "wt");
	if (!image_file) {
		return false;
	}

	// determines the new magic word
	// it depends on the format that the image body is saved to the file
	char magic_word[3];
	if (header.type == 1) {
		strcpy(magic_word, (mode ? "P1" : "P4"));
	} else if (header.type == 2) {
		strcpy(magic_word, (mode ? "P2" : "P5"));
	} else {
		strcpy(magic_word, (mode ? "P3" : "P6"));
	}

	// prints the image header data to <image_file>
	fprintf(image_file, "%s\n", magic_word);
	fprintf(image_file, "%d %d\n", header.width, header.height);
	if (header.type != 1) { // black and white images don't require this field
		fprintf(image_file, "%d\n", header.max_value);
	}

	// closes the image file
	fclose(image_file);
	return true;
}

bool save_body(const char *image_name, bool mode,
			   void **body, image_header_t header)
{
	// tries to open the file named <image_name> in ASCII/BINARY (append mode)
	FILE *image_file = fopen(image_name, (mode ? "at" : "ab"));
	if (!image_file) {
		return false;
	}

	// the image header data
	int height = header.height;
	int width = header.width;
	int type = header.type;

	/*
		prints the image body data to <image_file> in a specific format
		depending on the <mode>
	*/
	for (int i = 0; i < height; i++) {
		if (mode) {			// ASCII format
			for (int j = 0; j < width; j++) {
				if (type == 1) {
					fprintf(image_file, "%hhu ",
							VALUE(bwpixel_t, body, i, j));
				} else if (type == 2) {
					fprintf(image_file, "%hhu ",
							VALUE(gpixel_t, body, i, j));
				} else {
					fprintf(image_file, "%hhu ",
							VALUE(cpixel_t, body, i, j).red);
					fprintf(image_file, "%hhu ",
							VALUE(cpixel_t, body, i, j).green);
					fprintf(image_file, "%hhu ",
							VALUE(cpixel_t, body, i, j).blue);
				}
			}
			fprintf(image_file, "\n");
		} else {			// binary format
			if (type == 1) {
				fwrite((bwpixel_t *)(body[i]), sizeof(bwpixel_t), width,
					   image_file);
			} else if (type == 2) {
				fwrite((gpixel_t *)(body[i]), sizeof(gpixel_t), width,
					   image_file);
			} else {
				fwrite((cpixel_t *)(body[i]), sizeof(cpixel_t), width,
					   image_file);
			}
		}
	}

	// closes the image file
	fclose(image_file);
	return true;
}

bool check_save_params(int param_count, char params[][ARGUMENTS_DIM])
{
	/*
		the SAVE command is valid when the input consists of:
		1) the command itself and the file name
		2) the command itself, the file name and the ascii flag ("ascii")
	*/
	return ((param_count == 2) ||
			(param_count == 3 && strcmp(params[2], "ascii") == 0));
}
