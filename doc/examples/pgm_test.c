/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream
 *
 * Name: Pxtrix
 * Author: smgdream
 * License: MIT Licnese
 * Version: 0.1.8
 * Date: 2025-01-10
 */

#include "image.h"
#include <stdio.h>
#include "bmpimg.h"
#include "pgm.h"

int main(int argc, char **argv)
{
	Image *img = NULL;
	Pgm *p5 = NULL;
	Bmpimg *bmp = NULL;

	if (argc < 2)
		return 1;

	bmp = bmp_new(BMP_EMPTY);
	img = img_new(IMG_EMPTY);
	p5 = pgm_read(argv[1]);

	pgm_info(p5, stdout);
	img2bmp(pgm2img(p5, img), bmp);
	bmp_write(bmp, "out.bmp");

	img_free(img);
	bmp_free(bmp);
	pgm_free(p5);
	return 0;
}