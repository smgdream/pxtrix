// SPDX-License-Identifier: MIT
/* Copyright (c) 2025 Smgdream */

// envelope

#include <math.h>
#include "util.h"
#include "image.h"
#include "pic2img.h"
#include "bmpimg.h"
#include "pgm.h"
#include "render.h"

#define ct_x(x, img) ((uint32_t) ( (x) * 100.0 + ((img)->width >> 1) ))
#define ct_y(y, img) ((uint32_t) ( (y) * 100.0 + ((img)->height >> 1) ) )
#define ct_x_edge(img) ((double) ( ((img)->width >> 1) / 100.0 ) )
#define ct_y_edge(img) ((double) ( ((img)->height >> 1) / 100.0 ) )

int main(void)
{
	Image *img = NULL;
	Bmpimg *bmp = NULL;
	Pgm *pgm = NULL;
	uint8_t gray_cal(const Px_def *px);

	if ((img = img_new(1920 * 2, 1080* 2, 32, SRGB)) == NULL)
		return 1;
	if ((bmp = bmp_new(BMP_EMPTY)) == NULL)
		return 2;
	pgm = pgm_new(PGM_EMPTY);
	const Px_def white = { 1, 1, 1, 1 };
	const Px_def black = { 0, 0, 0, 1 };
	const Px_def gray   = rgba_4val(200, 200, 200, 255);
	const Px_def green = rgba_hex(0x00ff00ff);

	fill(img, white);
	for (double x = (double)(int)(-ct_x_edge(img)); x < ct_x_edge(img); ++x)
		if (x != 0)
			line_col(img, ct_x(x, img), 1, gray);
	for (double y = (double)(int)(-ct_y_edge(img)); y < ct_y_edge(img); ++y)
		if (y != 0)
			line_row(img, ct_y(y, img), 1, gray);
	line_col(img, ct_x(0, img), 4, black);
	line_row(img, ct_y(0, img), 4, black);		
	for (double x = -ct_x_edge(img); x < ct_x_edge(img); x += 1.0 / 6400.0)
		*img_px(img, ct_x(x, img), ct_y(cos(0.5 * x) * sin(10 * x), img)) = green;

	img2bmp(img, bmp);
	bmp_write(bmp, "out.bmp");
	//img2pgm(img, pgm, NULL);
	//pgm_write(pgm, "out.pgm");
	
	pgm_free(pgm);
	img_free(img);
	bmp_free(bmp);
	return 0;
}