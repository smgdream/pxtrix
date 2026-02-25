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
	Image *img1 = NULL;
	Bmpimg *bmp0 = NULL;
	Bmpimg *bmp1 = NULL;
	//Bmpimg *bmp2 = NULL; ///

	if (argc < 3) {
		fprintf(stderr, "too few arguments\n");
		return 1;
	}

	img0 = img_new(2880, 1620*2, 32, sRGB);
	img1 = img_new(IMG_EMPTY);
	bmp0 = bmp_read(argv[1]);
	bmp1 = bmp_read(argv[2]);
	//bmp2 = bmp_read(argv[3]); ///

	timer_start();

	bmp2img(bmp0, img1);
	cat(img0, img1, 0, 0);
	bmp2img(bmp1, img1);
	cat(img0, img1, 0, 1620);
	//bmp2img(bmp2, img1); ///
	//cat(img0, img1, 2048, 2430); ///

	printf("Time used: %.3fms\n", (double)time_all()/1000);

	img2bmp(img0, bmp0);
	bmp_write(bmp0, "out.bmp");

	img_free(img0);
	img_free(img1);
	bmp_free(bmp0);
	bmp_free(bmp1);
	//bmp_free(bmp2); ///
	return 0;
}