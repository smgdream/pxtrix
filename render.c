/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#include <stddef.h>
#include "render.h"
#include "image.h"

Image *gray(Image *img)
{
	if (img == NULL || img->buf == NULL)
		return NULL;
	
	uint8_t val = 0;
	size_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, img->width, img->height) {
		val = (uint8_t) (0.3 * img_px(img, x, y)->r + 0.59 * img_px(img, x, y)->g + 0.11 * img_px(img, x, y)->b);
		img_px(img, x, y)->r = val;
		img_px(img, x, y)->g = val;
		img_px(img, x, y)->b = val;
		img_px(img, x, y)->a = 255;
	}
	return img;
}
