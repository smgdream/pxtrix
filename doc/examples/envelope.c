/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

// envelope

//#include <stdio.h>
#include <math.h>
#include "util.h"
#include "image.h"
#include "bmpimg.h"
#include "pgm.h"
#include "render.h"

int main(void)
{
	Image *img = NULL;
	Bmpimg *bmp = NULL;
	Pgm *pgm = NULL;
	uint8_t gray_cal(const Px_def *px);

	if ((img = img_new(1920 * 2, 1080* 2, 32, sRGB)) == NULL)
		return 1;
	if ((bmp = bmp_new(BMP_EMPTY)) == NULL)
		return 2;
	pgm = pgm_new(PGM_EMPTY);
	const Px_def white = { 255, 255, 255, 255 };
	const Px_def black = { 0  , 0  , 0  , 255 };
	//const Px_def gra   = { 200, 200, 200, 255 };
	//const Px_def green = { 0  , 255, 0  , 255 };

	fill(img, &white);
	for (double x = (double)(int)(-ct_x_edge(img)); x < ct_x_edge(img); ++x)
		if (x != 0)
			line_col(img, ct_x(x, img), 1, obj_literal(Px_def, {211, 211, 211, 255}));
	for (double y = (double)(int)(-ct_y_edge(img)); y < ct_y_edge(img); ++y)
		if (y != 0)
			line_row(img, ct_y(y, img), 1, obj_literal(Px_def, {211, 211, 211, 255}));
	line_col(img, ct_x(0, img), 4, &black);
	line_row(img, ct_y(0, img), 4, &black);		
	for (double x = -ct_x_edge(img); x < ct_x_edge(img); x += 1.0 / 6400.0)
		*img_px(img, ct_x(x, img), ct_y(cos(0.5 * x) * sin(10 * x), img)) = *obj_literal(Px_def, {0, 255, 0, 255});

	//img2bmp(img, bmp);
	//bmp_write(bmp, "out.bmp");
	img2pgm(img, pgm, NULL);
	pgm_write(pgm, "out.pgm");
	
	pgm_free(pgm);
	img_free(img);
	bmp_free(bmp);
	return 0;
}

uint8_t gray_cal(const Px_def *px)
{
	return (uint8_t)(
		0.5 * px->r +
		0.0 * px->g +
		0.5 * px->b
	);
}