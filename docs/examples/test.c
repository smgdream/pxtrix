/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

#include <stdio.h>
#include "bmpimg.h"
#include "image.h"
#include "pic2img.h"
#include "render.h"
#include "util.h"

int main(int argc, char *argv[])
{
	Image *img0 = NULL;
	Bmpimg *bmp0 = NULL;

	if (argc < 2) {
		fprintf(stderr, "too few arguments\n");
		return 1;
	}

	img0 = img_new(IMG_EMPTY);
	bmp0 = bmp_read(argv[1]);
	bmp2img(bmp0, img0);

	timer_start();

	crop(img0, 512, 300, 800, 256);

	printf("Time used: %.3fms\n", (double)time_all()/1000);

	img2bmp(img0, bmp0);
	bmp_write(bmp0, "out.bmp");

	img_free(img0);
	bmp_free(bmp0);
	return 0;
}