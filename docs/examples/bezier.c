// SPDX-License-Identifier: MIT
/* Copyright (c) 2025 Smgdream */

// Bezier Curve

#include <stdio.h>
#include "image.h"
#include "pic2img.h"
#include "bmpimg.h"
#include "tmath.h"
#include "util.h"
#include "render.h"

int main(int argc, char **argv)
{
	Image *img = NULL;
	Bmpimg *bmp = NULL;
	char *name = NULL;
	img = img_new(1920, 1080, 32, SRGB);
	bmp = bmp_new(BMP_EMPTY);
	name = (argc > 1) ? argv[1] : "bezier.bmp";
	
	Vec2 p0 = { 0, 0 };
	Vec2 p1 = { 0, 0 };
	Vec2 p2 = { 0, 0 };
	Vec2 p3 = { 0, 0 };
	printf("(x1,y1) (x2,y2) (x3,y3) (x4,y4)\n");
	printf("list of vector: ");
	if (scanf("(%lf, %lf) (%lf, %lf) (%lf, %lf) (%lf, %lf)",
		&p0.x, &p0.y, &p1.x, &p1.y, &p2.x, &p2.y, &p3.x, &p3.y) != 8) {
			fprintf(stderr, "unexpected input.\n");
			return -1;
	}
	
	timer_start(); ///
	fill(img, rgba_hex(0xffffffff));
	bezier_curve(img, p0, p1, p2, p3, rgba_hex(0x000000ff));

	fprintf(stderr, "time: %.3f ms\n", (double) time_step()/1000.0); ///

	img2bmp(img, bmp);
	bmp_write(bmp, name);

	img_free(img);
	bmp_free(bmp);
	return 0;
}
