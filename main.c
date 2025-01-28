/*
 * Licensed under the MIT License
 * Copyright (c) 2025 Smgdream
 *
 * Name: pxtrix
 * Author: smgdream
 * License: MIT Licnese
 * Version: 0.1.9
 * Date: 2025-01-20
 */

// cardioid

// #include <stdio.h>
#include <math.h>
#include "image.h"
#include "bmpimg.h"
#include "render.h"
#include "tmath.h"
#include "util.h"


#define A (256)
#define min(num1, num2) ( ((num1) < (num2)) ? (num1) : (num2) )


int main(void)
{
	Image *img = img_new(1920, 1080, 32, sRGB);
	Bmpimg *bmp = bmp_new(BMP_EMPTY);
	double mini = min(img->width >> 1, img->height >> 1);

	fill(img, obj_literal(Px_def, {255, 255, 255, 255}));
	//timer_start();
	double r = 0.0, theta = 0.0;
	polar_for_each(r, theta, mini) {
		if (r < A*(1-sin(theta)))
			*img_px_c(img, r*cos(theta), r*sin(theta))
				= *obj_literal(Px_def, {100, 200, 16, 255});
	}
	//printf("Used: %lld us\n", time_all());
	img2bmp(img, bmp);
	bmp_write(bmp, "out.bmp");

	img_free(img);
	bmp_free(bmp);
	return 0;
}