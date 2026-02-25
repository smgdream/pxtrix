/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

#include "fbdev.h"
#include "image.h"

Image *fb2img(const Fbdev *fbd, Image *img)
{
	if (fbd == NULL || img == NULL || fbd->buf == NULL)
		return NULL;
	if (img->width != fbd->xres || img->height != fbd->yres)
		img_set(img, fbd->xres, fbd->yres, 32, sRGB);

	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, fbd->xres, fbd->yres) {
		img_px(img, x, y)->r = fb_px(fbd, x, fbd->yres-1-y)->r;
		img_px(img, x, y)->g = fb_px(fbd, x, fbd->yres-1-y)->g;
		img_px(img, x, y)->b = fb_px(fbd, x, fbd->yres-1-y)->b;
		img_px(img, x, y)->a = fb_px(fbd, x, fbd->yres-1-y)->a;
	}

	return img;
}

Fbdev *img2fb(const Image *img, Fbdev *fbd)
{
	if (img == NULL || fbd == NULL || img->buf == NULL)
		return NULL;
	if (fbd->dev_flag != 1 && (fbd->xres != img->width || fbd->yres != img->height))
		fb_set(fbd, img->width, img->height, 0);

	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, fbd->xres, fbd->yres) {
		fb_px(fbd, x, (fbd->yres-1-y))->r = img_px(img, x, y)->r;
		fb_px(fbd, x, (fbd->yres-1-y))->g = img_px(img, x, y)->g;
		fb_px(fbd, x, (fbd->yres-1-y))->b = img_px(img, x, y)->b;
		fb_px(fbd, x, (fbd->yres-1-y))->a = img_px(img, x, y)->a;
	}

	return fbd;
}