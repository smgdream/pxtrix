/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#include <stdio.h>
#include "image.h"
#include "bmpimg.h"

int main(int argc, char **argv)
{
	Image *img = NULL;
	Bmpimg *bmp = NULL;
	uint32_t x, y;
	
	if ((img = img_new(IMG_EMPTY)) == NULL)
		return 1;
	if ((bmp = bmp_read(argv[1])) == NULL)
		return 2;
	
	bmp2img(bmp, img);
	img_for_px(x, y, 0, 15, 256, 1)
		printf("\t{ %d, %d, %d, %d },\n", img_px(img, x, y)->r, img_px(img, x, y)->g, img_px(img, x, y)->b, img_px(img, x, y)->a);
	
	img_free(img);
	bmp_free(bmp);
	return 0;
}
