/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream
 *
 * Name: Pxtrix
 * Author: smgdream
 * License: MIT Licnese
 * Date: 2025-02-19
 */

/* Not support for tcc */

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "image.h"
#include "bmpimg.h"
#include "perf.h"
#include "pic2img.h"
#include "luts/lut_magma.h"

/* -2 <= real <= 2, -2 <= imag <= -2
 * nbl means: no belong */
int32_t nbl2mandelbrot(double real, double imag);
static int max_it = 0;

int main(int argc, char **argv)
{
	Bmpimg *b = NULL;
	Image *i = NULL;
	//Qoimg *q = NULL;
	 
	if (argc != 3) {
		fprintf(stderr, "too %s argument\n", (argc > 3) ? "many" : "few");
		fprintf(stderr, "arg1: size of image; arg2: gamma\n");
		fprintf(stderr, "e.g. program 1920 0.3\n");
		return 1;
	}

	max_it = atoi(argv[1]);
	if ((i = img_new(atoi(argv[1]), atoi(argv[1]), 24, sRGB)) == NULL)
		return 1;
	if ((b = bmp_new(BMP_EMPTY)) == NULL)
		return 2;

	timer_start(); //////
	int32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, i->width, i->height) {
		int32_t val = nbl2mandelbrot((double)(x - ((int64_t)i->width >> 1)) * 4.0 / (double)i->width,
									(double)(y - ((int64_t)i->height >> 1)) * 4.0 / (double)i->height);
		uint8_t clr = (val < 0) ? 0 : (uint8_t)(pow((double)val / (double)max_it, atof(argv[2])) * 255);
		*img_px(i, x, y) = clr_lut_magma[clr];

	}
	printf("Time of MDB draw: %.2f s\n", time_step() / 1000000.0); ///////////////

	img2bmp(i, b);
	bmp_write(b, "mdb.bmp");
	printf("Time of bmp out: %u us\n", time_step()); /////////

	img_free(i);
	bmp_free(b);
	return 0;
}
 
int32_t nbl2mandelbrot(double re, double im)
{
	complex z = 0 + 0 * I;
	complex c = re + im * I;
	 
	for (int i = 0; i < max_it; ++i, z = z * z + c)
		if (cabs(z) > 2)
			return i;
	return -1;
}
 