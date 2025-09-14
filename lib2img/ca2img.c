/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

#include "chart.h"
#include "image.h"

Chart *img2ca(const Image *img, Chart *ca)
{
	if (img == NULL || img->buf == NULL || ca == NULL)
		return NULL;
	if (ca->buf == NULL || ca->width != img->width || ca->height != img->height)
		ca_set(ca, img->width, img->height, ca->cols);
	
	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, ca->width, ca->height) {
		ca->buf[x + (ca->height-1-y) * ca->width].r = img_px(img, x, y)->r;
		ca->buf[x + (ca->height-1-y) * ca->width].g = img_px(img, x, y)->g;
		ca->buf[x + (ca->height-1-y) * ca->width].b = img_px(img, x, y)->b;
	}
	
	return ca;
}