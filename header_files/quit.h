// Tene Victor-Gabriel, 315CA

#pragma once

#include <stdbool.h>
#include "image_utils.h"

// quits the program gracefully while deallocating all the used resources
bool quit(void ***body, image_header_t *header, int param_count);

// checks the validity of the parameters of the QUIT command
bool check_quit_params(int param_count);
