/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

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
	img = img_new(1920, 1080, 32, sRGB);
	bmp = bmp_new(BMP_EMPTY);
	name = (argc > 1) ? argv[1] : "bezier.bmp";
	
	Vec2 p0 = { 0, 0 };
	Vec2 p1 = { 0, 0 };
	Vec2 p2 = { 0 ,0 };
	Vec2 p3 = { 0 ,0 };
	printf("pn: x y\n");
	printf("p0: ");
	scanf("%lf %lf", &p0.x, &p0.y);
	printf("p1: ");
	scanf("%lf %lf", &p1.x, &p1.y);
	printf("p2: ");
	scanf("%lf %lf", &p2.x, &p2.y);
	printf("p3: ");
	scanf("%lf %lf", &p3.x, &p3.y);
	
	timer_start(); ///
	fill(img, obj_literal(Px_def, { 255, 255, 255, 255 }));
	bezier_curve_c(img, p0, p1, p2, p3, obj_literal(Px_def, { 0, 0, 0, 255 }));

	fprintf(stderr, "time: %.3f ms\n", (double) time_step()/1000.0); ///

	img2bmp(img, bmp);
	bmp_write(bmp, name);

	img_free(img);
	bmp_free(bmp);
	return 0;
}
