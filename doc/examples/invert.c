/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

#include <stdio.h>
#include "bmpimg.h"
#include "image.h"
#include "pic2img.h"
#include "render.h"

int main(int argc, char *argv[])
{
	Image *img = NULL;
	Bmpimg *bmp = NULL;

	if (argc < 2) {
		fprintf(stderr, "too few arguments\n");
		return 1;
	}

	img = img_new(IMG_EMPTY);
	bmp = bmp_read(argv[1]);

	bmp2img(bmp, img);
	invert(img);
	img2bmp(img, bmp);
	bmp_write(bmp, "out.bmp");

	img_free(img);
	bmp_free(bmp);
	return 0;
}