// SPDX-License-Identifier: MIT
/* Copyright (c) 2024 Smgdream */

#include <stddef.h>
#include <stdint.h>
#include "bmpimg.h"
#include "image.h"
#include "pic2img.h"

Image *bmp2img(const Bmpimg *bmp, Image *img)
{
	uint32_t wid = 0, hgt = 0;

	if (bmp == NULL || bmp->buf == NULL || img == NULL)
		return NULL;

	wid = bmp->bih.bi_width;
	hgt = bmp->bih.bi_height;
	if (img_set(img, wid, hgt, bmp->bih.bi_bitcount, SRGB) == NULL)
		return NULL;

	size_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, wid, hgt) {
		img_px(img, x, y)->r = bmp_px(bmp, x, y)->r / 255.0f;
		img_px(img, x, y)->g = bmp_px(bmp, x, y)->g / 255.0f;
		img_px(img, x, y)->b = bmp_px(bmp, x, y)->b / 255.0f;
		img_px(img, x, y)->a = 1.0f;
	}
	return img;
}

Bmpimg *img2bmp(const Image *img, Bmpimg *bmp)
{
	if (img == NULL || img->buf == NULL || bmp == NULL)
		return NULL;

	if (bmp_set(bmp, img->width, img->height, 24) == NULL)
		return NULL;
	
	size_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, img->width, img->height) {
		bmp_px(bmp, x, y)->r = (uint8_t) (img_px(img, x, y)->r * 255.0f);
		bmp_px(bmp, x, y)->g = (uint8_t) (img_px(img, x, y)->g * 255.0f);
		bmp_px(bmp, x, y)->b = (uint8_t) (img_px(img, x, y)->b * 255.0f);
	}
	return bmp;
}
