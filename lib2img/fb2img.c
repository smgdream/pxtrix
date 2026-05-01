// SPDX-License-Identifier: MIT
/* Copyright (c) 2025 Smgdream */

#include <stddef.h>
#include <stdint.h>
#include "fbdev.h"
#include "image.h"
#include "pic2img.h"
#include "tmath.h"

Image *fb2img(const Fbdev *fbd, Image *img)
{
	if (fbd == NULL || img == NULL || fbd->buf == NULL)
		return NULL;
	if (img->width != fbd->xres || img->height != fbd->yres)
		img_set(img, fbd->xres, fbd->yres, 32, SRGB);

	uint32_t y_reverse = 0;
	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, fbd->xres, fbd->yres) {
		y_reverse = fbd->yres-1-y;
		img_px(img, x, y)->r = fb_px(fbd, x, y_reverse)->r / 255.0f;
		img_px(img, x, y)->g = fb_px(fbd, x, y_reverse)->g / 255.0f;
		img_px(img, x, y)->b = fb_px(fbd, x, y_reverse)->b / 255.0f;
		img_px(img, x, y)->a = 1.0f;
	}

	return img;
}

Fbdev *img2fb(const Image *img, Fbdev *fbd)
{
	if (img == NULL || fbd == NULL || img->buf == NULL)
		return NULL;
	if (fbd->dev_type != FB_DEVICE && (fbd->xres != img->width || fbd->yres != img->height))
		fb_set(fbd, img->width, img->height, FB_FILE);

	uint32_t y_reverse = 0;
	uint32_t x = 0, y = 0;
	uint32_t wid = min(img->width, fbd->xres);
	uint32_t hgt = min(img->height, fbd->yres);
	img_for_px(x, y, 0, 0, wid, hgt) {
		y_reverse = fbd->yres-1-y;
		fb_px(fbd, x, y_reverse)->r = (uint8_t) (img_px(img, x, y)->r * 255.0f);
		fb_px(fbd, x, y_reverse)->g = (uint8_t) (img_px(img, x, y)->g * 255.0f);
		fb_px(fbd, x, y_reverse)->b = (uint8_t) (img_px(img, x, y)->b * 255.0f);
		fb_px(fbd, x, y_reverse)->a = 255;
	}

	return fbd;
}