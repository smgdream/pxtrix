// SPDX-License-Identifier: MIT
/* Copyright (c) 2024 Smgdream */

#include <stddef.h>
#include <stdint.h>
#include "image.h"
#include "qoimg.h"
#include "pic2img.h"

Image *qoi2img(const Qoimg *qoi, Image *img)
{
	if (qoi == NULL || qoi->buf == NULL || img == NULL)
		return NULL;

	img_set(img,
			qoi->header.width,
			qoi->header.height,
			qoi->header.channels * 8,
			(qoi->header.colorspace == QOI_GAMUT_SRGB_LA) ? SRGB : UNO_LINEAR);
	
	uint32_t y_reverse = 0;
	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, img->width, img->height) {
		y_reverse = qoi->header.height-1-y;
		img_px(img, x, y)->r = qoi_px(qoi, x, y_reverse)->r / 255.0f;
		img_px(img, x, y)->g = qoi_px(qoi, x, y_reverse)->g / 255.0f;
		img_px(img, x, y)->b = qoi_px(qoi, x, y_reverse)->b / 255.0f;
		img_px(img, x, y)->a = qoi_px(qoi, x, y_reverse)->a / 255.0f;
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
			(img->bpp == 24) ? QOI_CH_RGB : QOI_CH_RGBA ,
			(img->gamut == UNO_LINEAR || img->gamut == ACESCG) ? 
							QOI_GAMUT_ALL_LINEAR : QOI_GAMUT_SRGB_LA );

	uint32_t y_reverse = 0;
	uint32_t x = 0, y = 0;
	/* To make it compatible with RGB and RGBA pixel and make it to convert faster.
	 * It need two branch to process, rather than ?: operator.  */
	if (img->bpp == 32) // RGBA
		img_for_px(x, y, 0, 0, img->width, img->height) {
			y_reverse = qoi->header.height-1-y;
			qoi_px(qoi, x, y_reverse)->r = (uint8_t) (img_px(img, x, y)->r * 255.0f);
			qoi_px(qoi, x, y_reverse)->g = (uint8_t) (img_px(img, x, y)->g * 255.0f);
			qoi_px(qoi, x, y_reverse)->b = (uint8_t) (img_px(img, x, y)->b * 255.0f);
			qoi_px(qoi, x, y_reverse)->a = (uint8_t) (img_px(img, x, y)->a * 255.0f);
		}
	else // RGB
		img_for_px(x, y, 0, 0, img->width, img->height) {
			y_reverse = qoi->header.height-1-y;
			qoi_px(qoi, x, y_reverse)->r = (uint8_t) (img_px(img, x, y)->r * 255.0f);
			qoi_px(qoi, x, y_reverse)->g = (uint8_t) (img_px(img, x, y)->g * 255.0f);
			qoi_px(qoi, x, y_reverse)->b = (uint8_t) (img_px(img, x, y)->b * 255.0f);
			qoi_px(qoi, x, y_reverse)->a = 255;
		}

	return qoi;
}
