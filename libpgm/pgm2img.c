/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

#define __LIBPGM_INSIDE__
#define __LIBPGM_USE_IMAGE__
#include <stddef.h>
#include "image.h"
#include "pgm.h"

static inline uint8_t
gray_cal(const Px_def *px)
{
	return (uint8_t)(
		0.333 * px->r +
		0.333 * px->g +
		0.333 * px->b
	);
}

Image *pgm2img(const Pgm *p5, Image *img)
{
	uint32_t x = 0, y = 0;
	uint8_t gray = 0;
	
	if (p5 == NULL || img == NULL)
		return NULL;
	if (img->width != p5->width || img->height != p5->height)
		img_set(img, p5->width, p5->height, 8, sRGB);
		
	img_for_px(x, y, 0, 0, p5->width, p5->height) {
		gray = p5->buf[x + (p5->height - 1 - y) * p5->width];
		img_px(img, x, y)->r = gray;
		img_px(img, x, y)->g = gray;
		img_px(img, x, y)->b = gray;
		img_px(img, x, y)->a = 255;
	}
	return img;
}

Pgm *img2pgm(const Image *img, Pgm *p5, uint8_t (*gray)(const Px_def *px))
{
	uint32_t x = 0, y = 0;
	uint8_t gray_val = 0;

	if (img == NULL || p5 == NULL)
		return NULL;
	if (p5->width != img->width || p5->height != img->height)
		pgm_set(p5, img->width, img->height);

	img_for_px(x, y, 0, 0, img->width, img->height) {
		gray_val = ((gray == NULL) ? gray_cal : gray)(img_px(img, x, y));
		p5->buf[x + (img->height - 1 - y) * img->width] = gray_val;
	}
	return p5;
}

