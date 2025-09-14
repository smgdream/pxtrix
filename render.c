/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "render.h"
#include "image.h"
#include "tmath.h"

static inline int px_avg(Px_def *px)
{
	return ((int)px->r + (int)px->g + (int)px->b) / 3;
}

static inline uint8_t gray_px_cal(const Px_def *px)
{
	return (uint8_t)(
		  0.30 * px->r
		+ 0.59 * px->g
		+ 0.11 * px->b
	);
}

static inline int
_row_swap(Image *img, uint32_t y0, uint32_t y1, size_t width, void *linebuf)
{
	// no test
	memmove(linebuf,				&img->buf[y0 * width],	width * sizeof(Px_def));
	memmove(&img->buf[y0 * width],	&img->buf[y1 * width],	width * sizeof(Px_def));
	memmove(&img->buf[y1 * width],	linebuf,				width * sizeof(Px_def));
	return 0;
}

Image *gray(Image *img, uint8_t (*f)(const Px_def *px))
{
	if (img == NULL || img->buf == NULL)
		return NULL;
	
	uint8_t val = 0;
	size_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, img->width, img->height) {
		val = (f != NULL) ? f(img_px(img, x, y)) : gray_px_cal(img_px(img, x, y));
		img_px(img, x, y)->r = val;
		img_px(img, x, y)->g = val;
		img_px(img, x, y)->b = val;
		img_px(img, x, y)->a = 255;
	}
	return img;
}

Image *fill(Image *img, const Px_def *clr)
{
	uint32_t x = 0, y = 0;

	if (img == NULL || clr == NULL)
		return NULL;
	img_for_px(x, y, 0, 0, img->width, img->height)
		*img_px(img, x, y) = *clr;

	return img;
}

Image *bin(Image *img, uint8_t threshold)
{
	uint32_t x = 0, y = 0;
	const Px_def black = { 0, 0, 0, 255 };
	const Px_def white = { 255, 255, 255, 255 };

	img_for_px(x, y, 0, 0, img->width, img->height)
		*img_px(img, x, y) = (px_avg(img_px(img, x, y)) >= threshold) ? white : black ;
	
	return img;
}

Image *line_row(Image *img, uint32_t y_coor, uint32_t weight, const Px_def *clr)
{
	uint32_t x = 0, y = 0;

	if (y_coor >= weight / 2)
		y_coor -= weight / 2;
	else {
		weight = weight / 2 + y_coor;
		y_coor = 0;
	}

	img_for_px(x, y, 0, y_coor, img->width, weight)
		*img_px(img, x, y) = *clr;

	return img;
}

Image *line_col(Image *img, uint32_t x_coor, uint32_t weight, const Px_def *clr)
{
	
	uint32_t x = 0, y = 0;
	
	if (x_coor >= weight / 2)
		x_coor -= weight / 2;
	else {
		weight = weight / 2 + x_coor;
		x_coor = 0;
	}

	img_for_px(x, y, x_coor, 0, weight, img->height)
		*img_px(img, x, y) = *clr;

	return img;
}

Image *bezier_curve_c(Image *img, Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, const Px_def *clr)
{
	if (img == NULL)
		return NULL;
	for (double t = 0; t <= 1; t += 0.0001)
		*img_px_v2(img, bezier_c(p0, p1, p2, p3, t)) = *clr;
	return img;
}

Image *scale(Image *img, uint32_t wid, uint32_t hgt)
{
	Image *tmp = NULL;

	if (img == NULL || img->buf == NULL)
		return NULL;
	tmp = img_new(wid, hgt, img->bpp, img->gamut);
	if (tmp == NULL)
		return NULL;

	/* vector scalar multiplication */
	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, img->width, img->height)
		*img_px(tmp,
			   (uint32_t) ((double)wid/img->width  * x),
			   (uint32_t) ((double)hgt/img->height * y)
		) = *img_px(img, x, y);

	img_copy(img, tmp);
	img_free(tmp);
	return img;
}

Image *crop(Image *img,
			uint32_t x , uint32_t y,
			uint32_t wid, uint32_t hgt)
{
	Image *tmp = NULL;

	if (img == NULL)
		return NULL;
	tmp = img_new(wid, hgt, img->bpp, img->gamut);
	if (tmp == NULL)
		return NULL;

	uint32_t it_x = 0, it_y = 0;
	img_for_px(it_x, it_y, 0, 0, wid, hgt)
		*img_px(tmp, it_x, it_y) = *img_px(img, x+it_x, y+it_y);

	img_copy(img, tmp);
	img_free(tmp);
	return img;
}

Image *cat(Image *dest, const Image *src, uint32_t x, uint32_t y)
{
	if (src == NULL || dest == NULL)
		return NULL;

	uint32_t it_x = 0, it_y = 0;
	img_for_px(it_x, it_y, 0, 0, src->width, src->height) {
		if (x+it_x < dest->width && y+it_y < dest->height)
			*img_px(dest, x+it_x, y+it_y) = *img_px(src, it_x, it_y);
	}

	return dest;
}

Image *invert(Image *img)
{
	if (img == NULL)
		return NULL;

	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, img->width, img->height) {
		img_px(img, x, y)->r = 255 - img_px(img, x, y)->r;
		img_px(img, x, y)->g = 255 - img_px(img, x, y)->g;
		img_px(img, x, y)->b = 255 - img_px(img, x, y)->b;
	}

	return img;
}

typedef unsigned char Byte;

Image *filp_v(Image *img)
{
	if (img == NULL)
		return NULL;

	uint32_t width = img->width;
	Byte *buf = malloc(width * sizeof(Px_def) * sizeof(Byte));

	if (buf == NULL)
		return NULL;
	for (uint32_t a = 0, b = img->height-1; a < b; ++a, --b)
		_row_swap(img, a, b, width, buf);

	return img;
}