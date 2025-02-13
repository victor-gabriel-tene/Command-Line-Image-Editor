// Tene Victor-Gabriel, 315CA

#include <stdbool.h>

#include "quit.h"
#include "image_utils.h"
#include "errors.h"

bool quit(void ***body, image_header_t *header, int param_count)
{
	// checks whether the parameters of the command are valid or not
	if (!check_quit_params(param_count)) {
		invalid_command_error();
		return false;
	}

	// checks whether the image is loaded or not
	if (!(*body)) {
		no_image_error();
		return false;
	}

	/*
		clears the image by deallocating the memory used for the body
		and reinitialising the header
	*/
	clear_image(body, header);
	return true;
}

bool check_quit_params(int param_count)
{
	/*
		the QUIT command is valid only when there is a single parameter:
		- the command itself
	*/
	return param_count == 1;
}
