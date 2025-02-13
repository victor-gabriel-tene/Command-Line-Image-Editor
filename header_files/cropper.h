// Tene Victor-Gabriel, 315CA

#pragma once

#include "image_utils.h"

// crops the image to the current selection
void crop_image(void ***body, image_header_t *header, area_t *area,
				int param_count);

// checks the validity of the parameters of the CROP command
bool check_crop_params(int param_count);
