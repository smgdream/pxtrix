/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#define __LIBQOI_INSIDE__
#define __LIBQOI_USE_IMAGE__
#include <stddef.h>
#include "image.h"
#include "qoimg.h"

Image *qoi2img(const Qoimg *qoi, Image *img)
{
	if (qoi == NULL || qoi->buf == NULL || img == NULL)
		return NULL;

	img_set(img,
			qoi->header.width,
			qoi->header.height,
			qoi->header.channels * 8,
			sRGB);
	size_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, img->width, img->height) {
		img_px(img, x, y)->r = qoi->buf[x + (img->height-1-y) * img->width].r;
		img_px(img, x, y)->g = qoi->buf[x + (img->height-1-y) * img->width].g;
		img_px(img, x, y)->b = qoi->buf[x + (img->height-1-y) * img->width].b;
		img_px(img, x, y)->a = qoi->buf[x + (img->height-1-y) * img->width].a;
	}

	return img;
}

Qoimg *img2qoi(const Image *img, Qoimg *qoi)
{
	if (img == NULL || img->buf == NULL || qoi == NULL)
		return NULL;
	
	qoi_set(qoi,
			img->width,
			img->height,
			img->bpp / 8,
			(img->gamut == sRGB) ? 0 : 1);

	size_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, img->width, img->height) {
		qoi->buf[x + (img->height-1-y) * img->width].r = img_px(img, x, y)->r;
		qoi->buf[x + (img->height-1-y) * img->width].g = img_px(img, x, y)->g;
		qoi->buf[x + (img->height-1-y) * img->width].b = img_px(img, x, y)->b;
		qoi->buf[x + (img->height-1-y) * img->width].a = img_px(img, x, y)->a;
	}

	return qoi;
}
