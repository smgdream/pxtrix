/*
 * Licensed under the MIT License
 * Copyright (c) 2024 Smgdream
 *
 * Name: pxtrix
 * Author: smgdream
 * License: MIT Licnese
 * Version: 0.1.6
 * Date: 2024-11-13
 */

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "image.h"
#include "bmpimg.h"
#include "qoimg.h"
#include "perf.h"

/* -2 <= real <= 2, -2 <= imag <= -2 */
int32_t in_mandelbrot(double real, double imag);
static int max_it = 0;

int main(int argc, char **argv)
{
	Bmpimg *b = NULL;
	Image *i = NULL;
	Qoimg *q = NULL;
	
	max_it = atoi(argv[1]);
	if ((i = img_new(atoi(argv[1]), atoi(argv[1]), 24, sRGB)) == NULL)
		return 1;
	if ((b = bmp_new(BMP_EMPTY)) == NULL)
		return 2;
	if ((q = qoi_new(QOI_EMPTY)) == NULL)
		return 3;
	timer_start(); //////
	int32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, i->width, i->height) {
		int32_t val = in_mandelbrot( (double)(x - ((int64_t)i->width >> 1)) * 4.0 / (double)i->width, (double)(y - ((int64_t)i->height >> 1)) * 4.0 / (double)i->height);
		uint8_t clr = (val < 0) ? 0 : (uint8_t)(pow((double)val / (double)max_it, atof(argv[2])) * 255) ;
		img_px(i, x, y)->r = clr;
		img_px(i, x, y)->g = clr;
		img_px(i, x, y)->b = clr;
		img_px(i, x, y)->a = 255;
		//printf("(%f, %f)\n", (double)(x - (4096 >> 1)) * 4.0 / 4096.0, (double)(y - (4096 >> 1)) * 4.0 / 4096.0);
	}
	printf("Time of MDB draw: %.2f s\n", time_step() / 1000000.0); ///////////////

	img2bmp(i, b);
	bmp_write(b, "mdb.bmp");
	printf("Time of bmp out: %u us\n", time_step()); /////////

	img2qoi(i, q);
	qoi_write(q, "mdb.qoi");
	printf("Time of qoi out: %u us\n", time_step()); /////////

	img_free(i);
	bmp_free(b);
	qoi_free(q);
	return 0;
}

#define MAX_IT (8192)

int32_t in_mandelbrot(double re, double im)
{
	complex z = 0 + 0 * I;
	complex c = re + im * I;
	
	for (int i = 0; i < max_it; ++i, z = z * z + c)
		if (cabs(z) > 2)
			return i;
	return -1;
}
