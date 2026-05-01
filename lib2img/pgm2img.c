// SPDX-License-Identifier: MIT
/* Copyright (c) 2025 Smgdream */

#include <stddef.h>
#include <stdint.h>
#include "image.h"
#include "pgm.h"
#include "pic2img.h"

static float gray_cal(Px_def px)
{
	/* ITU-R BT.709-6 */
	return	0.2126f / 1.005f * px.r +
			0.7152f / 1.005f * px.g +
			0.0772f / 1.005f * px.b;
}

Image *pgm2img(const Pgm *p5, Image *img)
{
	uint32_t x = 0, y = 0;
	float gray = 0;
	
	if (p5 == NULL || p5->buf == NULL || img == NULL)
		return NULL;
	if (img->width != p5->width || img->height != p5->height)
		img_set(img, p5->width, p5->height, 8, SRGB);
		
	img_for_px(x, y, 0, 0, p5->width, p5->height) {
		gray = p5->buf[x + (p5->height - 1 - y) * p5->width] / 65535.0f;
		img_px(img, x, y)->r = gray;
		img_px(img, x, y)->g = gray;
		img_px(img, x, y)->b = gray;
		img_px(img, x, y)->a = 1.0f;
	}
	return img;
}

Pgm *img2pgm(const Image *img, Pgm *p5, float (*gris)(Px_def px))
{
	uint32_t x = 0, y = 0;
	uint16_t gray_val = 0;

	if (img == NULL || img->buf == NULL || p5 == NULL)
		return NULL;
	if (p5->width != img->width || p5->height != img->height)
		pgm_set(p5, img->width, img->height, p5->maxval);

	img_for_px(x, y, 0, 0, img->width, img->height) {
		gray_val = (uint16_t) ( 65535.0f * ((gris == NULL) ? gray_cal : gris)(*img_px(img, x, y)) );
		p5->buf[x + (p5->height - 1 - y) * p5->width] = gray_val;
	}
	return p5;
}