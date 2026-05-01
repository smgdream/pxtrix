// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#include <stdio.h>
#include <inttypes.h>
#include "image.h"
#include "qoimg.h"
#include "pic2img.h"
#include "luts/luts.h"

int main(int argc, char **argv)
{
	Image *img = NULL;
	Qoimg *out = NULL;
	
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <outfile.qoi>\n", argv[0]);
		return __LINE__;
	}

	if ((img = img_new(1920, 1080, 32, SRGB)) == NULL) {
		fprintf(stderr, "error fail to new a Image object\n");
		return __LINE__;
	}
	if ((out = qoi_new(QOI_EMPTY)) == NULL) {
		fprintf(stderr, "error fail to new a Qoimg object\n");
		return __LINE__;
	}

	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, 1920, 1080) {
		switch ((int) (y / 1080.0 * 4.0)) {
			case 0:
				*img_px(img, x, y) = lut_inferno((float)x / (1920-1));
				break;
			case 1:
				*img_px(img, x, y) = lut_magma((float)x / (1920-1));
				break;
			case 2:
				*img_px(img, x, y) = lut_hot((float)x / (1920-1));
				break;
			case 3:
				*img_px(img, x, y) = lut_hsv((float)x / (1920-1));
				break;
			
		}
	}

	if (img2qoi(img, out) == NULL) {
		fprintf(stderr, "error fail to convert Image to Qoimg\n");
		return __LINE__;
	}
	if (qoi_write(out, argv[1]) != 0) {
		fprintf(stderr, "error fail to write a QOI file\n");
		perror("");
		return __LINE__;
	}

	if (img_free(img) != 0)
		return __LINE__;
	if (qoi_free(out) != 0)
		return __LINE__;
	return 0;
}