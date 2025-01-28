/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#include <stddef.h>
#include "render.h"
#include "image.h"

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
	Px_def black = { 0, 0, 0, 255 };
	Px_def white = { 255, 255, 255, 255 };

	img_for_px(x, y, 0, 0, img->width, img->height)
		*img_px(img, x, y) = px_avg(img_px(img, x, y)) >= threshold ? white : black ;
	
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