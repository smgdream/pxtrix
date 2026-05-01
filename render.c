// SPDX-License-Identifier: MIT
/* Copyright (c) 2024 Smgdream */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "render.h"
#include "image.h"
#include "tmath.h"

static float gray_px_cal(Px_def px)
{
	/* ITU-R BT.709-6 */
	return 0.2126 / 1.005f * px.r
		 + 0.7152 / 1.005f * px.g
		 + 0.0722 / 1.005f * px.b;
}

Image *gray(Image *img, float (*gris)(Px_def px))
{
	if (img == NULL || img_is_empty(img))
		return NULL;
	
	float val = 0;
	size_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, img->width, img->height) {
		val = ((gris != NULL) ? gris : gray_px_cal)(*img_px(img, x, y));
		img_px(img, x, y)->r = val;
		img_px(img, x, y)->g = val;
		img_px(img, x, y)->b = val;
	}
	return img;
}

Image *bin(Image *img, float (*gris)(Px_def px), float threshold)
{
	const Px_def black = { 0, 0, 0, 1 };
	const Px_def white = { 1, 1, 1, 1 };

	if (img == NULL || img_is_empty(img))
		return NULL;

	gray(img, gris);
	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, img->width, img->height)
		*img_px(img, x, y) = (img_px(img, x, y)->r >= threshold) ? white : black ;
	
	return img;
}

Image *fill(Image *img, Px_def clr)
{
	if (img == NULL || img_is_empty(img))
		return NULL;

	uint32_t x = 0, y = 0;	
	img_for_px(x, y, 0, 0, img->width, img->height)
		*img_px(img, x, y) = clr;

	return img;
}

Image *line_row(Image *img, uint32_t y_coor, uint32_t weight, Px_def clr)
{
	if (img == NULL || img_is_empty(img))
		return NULL;
	
	if (y_coor >= weight / 2)
		y_coor -= weight / 2;
	else {
		weight = weight / 2 + y_coor;
		y_coor = 0;
	}

	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, y_coor, img->width, weight)
		*img_px(img, x, y) = alpha_blend(*img_px(img, x, y), clr);

	return img;
}

Image *line_col(Image *img, uint32_t x_coor, uint32_t weight, Px_def clr)
{
	if (img == NULL || img_is_empty(img))
		return NULL;
	
	if (x_coor >= weight / 2)
		x_coor -= weight / 2;
	else {
		weight = weight / 2 + x_coor;
		x_coor = 0;
	}

	uint32_t x = 0, y = 0;
	img_for_px(x, y, x_coor, 0, weight, img->height)
		*img_px(img, x, y) = alpha_blend(*img_px(img, x, y), clr);

	return img;
}

Image *bezier_curve(Image *img, Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, Px_def clr)
{
	if (img == NULL || img_is_empty(img))
		return NULL;
	for (double t = 0; t <= 1; t += 0.0001) {
		Vec2 vec = bezier_c(p0, p1, p2, p3, t);
		if (vec.x < img->width && vec.y < img->height)
			*img_px_v2(img, vec) = alpha_blend(*img_px_v2(img, vec), clr);;
	}
	return img;
}

Image *scale(Image *img, uint32_t wid, uint32_t hgt)
{
	Image *tmp = NULL;

	if (img == NULL || img_is_empty(img))
		return NULL;
	tmp = img_new(wid, hgt, img->bpp, img->gamut);
	if (tmp == NULL)
		return NULL;
	
	/* Nearest neighbor interpolation */
	float scale_x = (float) wid / img->width, 
		  scale_y = (float) hgt / img->height;
	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, wid, hgt)
		*img_px(tmp, x, y) = *img_px(img, (uint32_t)(x/scale_x), (uint32_t) (y/scale_y));

	img_copy(img, tmp);
	img_free(tmp);
	return img;
}

Image *crop(Image *img,
			uint32_t x , uint32_t y,
			uint32_t wid, uint32_t hgt)
{
	Image *tmp = NULL;

	if (img == NULL || img_is_empty(img) || x >= img->width || y >= img->height)
		return NULL;
	if (x+wid > img->width || y+hgt > img->height) {
		wid = img->width - x;
		hgt = img->height - y;
	}
	
	if ((tmp = img_new(wid, hgt, img->bpp, img->gamut)) == NULL)
		return NULL;

	uint32_t it_x = 0, it_y = 0;
	img_for_px(it_x, it_y, 0, 0, wid, hgt)
		*img_px(tmp, it_x, it_y) = *img_px(img, x+it_x, y+it_y);

	img_copy(img, tmp);
	img_free(tmp);
	return img;
}

Image *overlay(Image *dest, const Image *src, int64_t x, int64_t y)
{
	if (dest == NULL || src == NULL || img_is_empty(dest) || img_is_empty(src))
		return NULL;

	uint32_t it_x = 0, it_y = 0;
	img_for_px(it_x, it_y, 0, 0, src->width, src->height)
		if (x+it_x >= 0 && x+it_x < dest->width 
		 && y+it_y >= 0 && y+it_y < dest->height)
			*img_px(dest, x+it_x, y+it_y) = alpha_blend(*img_px(dest, x+it_x, y+it_y),
														*img_px(src, it_x, it_y));

	return dest;
}

Image *invert(Image *img)
{
	if (img == NULL || img_is_empty(img))
		return NULL;

	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, img->width, img->height) {
		img_px(img, x, y)->r = 1.0f - img_px(img, x, y)->r;
		img_px(img, x, y)->g = 1.0f - img_px(img, x, y)->g;
		img_px(img, x, y)->b = 1.0f - img_px(img, x, y)->b;
	}

	return img;
}

typedef unsigned char Byte;

static inline int
_row_swap(Image *img, uint32_t y0, uint32_t y1, size_t width, Byte *linebuf)
{
	/* function user must make sure img and linebuf is not NULL */
	memmove(linebuf,				&img->buf[y0 * width],	width * sizeof(Px_def));
	memmove(&img->buf[y0 * width],	&img->buf[y1 * width],	width * sizeof(Px_def));
	memmove(&img->buf[y1 * width],	linebuf,				width * sizeof(Px_def));
	return 0;
}

Image *flip_v(Image *img)
{
	if (img == NULL || img_is_empty(img))
		return NULL;

	uint32_t width = img->width;
	Byte *buf = malloc(width * sizeof(Px_def) * sizeof(Byte));

	if (buf == NULL)
		return NULL;
	for (uint32_t a = 0, b = img->height-1; a < b; ++a, --b)
		_row_swap(img, a, b, width, buf);

	free(buf);
	return img;
}

Image *flip_h(Image *img)
{
	if (img == NULL || img_is_empty(img))
		return NULL;

	for (uint32_t y = 0; y < img->height; ++y)
		for (uint32_t i = 0, j = img->width-1; i < j; ++i, --j) {
			Px_rgba tmp = *img_px(img, i, y);
			*img_px(img, i, y) = *img_px(img, j, y);
			*img_px(img, j, y) = tmp;
		}
	
	return img;
}