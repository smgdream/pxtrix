// SPDX-License-Identifier: MIT
/* Copyright (c) 2025 Smgdream */

#include <stddef.h>
#include <stdint.h>
#include "chart.h"
#include "image.h"
#include "pic2img.h"

static inline uint8_t _gray(Px_rgba *px)
{
	return (uint8_t) (255.0f * (
		   0.2126 / 1.005f * px->r
		 + 0.7152 / 1.005f * px->g
		 + 0.0722 / 1.005f * px->b));
}

Chart *img2ca(const Image *img, Chart *ca)
{
	if (img == NULL || img->buf == NULL || ca == NULL)
		return NULL;
	if (ca->buf == NULL || ca->width != img->width || ca->height != img->height)
		ca_set(ca, img->width, img->height, ca->cols);
	
	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, ca->width, ca->height)
		ca->buf[x + (ca->height-1-y) * ca->width] = _gray(img_px(img, x, y));
	
	return ca;
}