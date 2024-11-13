/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"
#include "bmpimg.h"
#include "qoimg.h"
#include "render.h"
#include "perf.h"

int main(int argc, char **argv)
{
	Image *img = NULL;
	Bmpimg *bmp = NULL, *bmp1 = NULL;
	Qoimg *qoi = NULL, *qoi1 = NULL;
	uint32_t x = 0, y = 0;

	//if (argc < 2)
	//	return 10;

	if ((img = img_new(1920, 1080, 32, sRGB)) == NULL)
		return 1;
	//if ((bmp = bmp_read(argv[1])) == NULL)
	//	return 2;
	if ((bmp1 = bmp_new(BMP_EMPTY)) == NULL)
		return 2;
	//if ((qoi = qoi_read(argv[1])) == NULL)
	//	return 3;
	//if ((qoi1 = qoi_read(argv[1])) == NULL)
	//	return 3;

	
	//bmp2img(bmp, img);
	//qoi2img(qoi, img);
	//gray(img);
	img_for_px(x, y, 0, 0, 1920, 1080) {
		img_px(img, x, y)->r = 255;
		img_px(img, x, y)->a = 255;
	}
	img2bmp(img, bmp1);
	//img2qoi(img, qoi1);

	//qoi_write(qoi1, "out.qoi");
	bmp_write(bmp1, "out.bmp");

	img_free(img);
	bmp_free(bmp);
	bmp_free(bmp1);
	qoi_free(qoi);
	qoi_free(qoi1);
	return 0;
}
