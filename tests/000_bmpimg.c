// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#include <stdio.h>
#include "bmpimg.h"
#include "tmath.h"

int main(int argc, char *argv[])
{
	Bmpimg *in = NULL;
	Bmpimg *out = NULL;

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
	
	if (bmp_info(in, stdout) != 0)
		return __LINE__;

	/* copy a bmp image */
	if (bmp_set(out, in->bih.bi_width, in->bih.bi_height, in->bih.bi_bitcount) == NULL)
		return __LINE__;
	for (int32_t y = 0; y < out->bih.bi_height; ++y)
		for (int32_t x = 0; x < out->bih.bi_width; ++x)
			*bmp_px(out, x, y) = *bmp_px(in, x, y);

	/* draw a line */
	Bmp_px_def px = {.r = 0x39, .g = 0xc5, .b = 0xbb};
	int32_t min_size = min(in->bih.bi_width, in->bih.bi_height);
	for (int32_t i = 0; i < min_size; ++i)
		*bmp_px(out, i ,i) = px;
	
	if (bmp_write(out, argv[2]) != 0) {
		fprintf(stderr, "error fail to write a Bmp file\n");
		return __LINE__;
	}

	if (bmp_free(in) != 0 || bmp_free(out) != 0)
		return __LINE__;
	return 0;
}