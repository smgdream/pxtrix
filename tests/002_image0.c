// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#include <stdio.h>
#include "image.h"
#include "bmpimg.h"
#include "pic2img.h"
#include "tmath.h"

int main(int argc, char *argv[])
{
	Bmpimg *in = NULL;
	Bmpimg *out = NULL;
	Image *tmp0 = NULL;
	Image *tmp1 = NULL;

	if (argc != 3) {
		fprintf(stderr, "too few argument\n\n%s <input file> <output file>\n", argv[0]);
		return __LINE__;
	}

	if ((in = bmp_read(argv[1])) == NULL) {
		fprintf(stderr, "error fail to read %s\n", argv[1]);
		return __LINE__;
	}
	if ((out = bmp_new(BMP_EMPTY)) == NULL) {
		fprintf(stderr, "error fail to create a Bmpimg object\n");
		return __LINE__;
	}
	if ((tmp0 = img_new(IMG_EMPTY)) == NULL) {
		fprintf(stderr, "error fail to create a Image object\n");
		return __LINE__;
	}
	if ((tmp1 = img_new(691, 31, 32, UNO_LINEAR)) == NULL) {
		fprintf(stderr, "error fail to create a Image object\n");
		return __LINE__;
	}

	if (img_set(tmp1, 111, 111, 24, OPRGB) == NULL) {
		fprintf(stderr, "error fail to reset Image\n");
		return __LINE__;
	}

	if (bmp2img(in, tmp0) == NULL) {
		fprintf(stderr, "error convert Bmpimg to Image fail\n");
		return __LINE__;
	}

	if (img_info(tmp0, stdout) != 0)
		return __LINE__;

	Px_def px = rgba_hex(0x39c5bbff);
	uint32_t min_size = min(tmp0->width, tmp0->height);
	uint32_t a = 0, b = 0, i = 0;
	img_for_px(a, b, 0, 0, tmp0->width, tmp0->height)
		if (i < min_size) {
			*img_px(tmp0, i, i) = px;
			++i;
		}

	if (img_copy(tmp1, tmp0) == NULL) {
		fprintf(stderr, "error fail to copy a Image\n");
		return __LINE__;
	}

	if (img2bmp(tmp1, out) == NULL) {
		fprintf(stderr, "error convert Image to Bmpimg fail\n");
		return __LINE__;
	}

	if (bmp_write(out, argv[2]) != 0) {
		fprintf(stderr, "error fail to write a Bmp file\n");
		return __LINE__;
	}

	if (img_free(tmp0) != 0 || img_free(tmp1) != 0)
		return __LINE__;
	if (bmp_free(in) != 0 || bmp_free(out) != 0)
		return __LINE__;
	return 0;
}