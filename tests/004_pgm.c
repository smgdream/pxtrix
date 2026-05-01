// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#include <stdio.h>
#include <string.h>
#include "image.h"
#include "pgm.h"
#include "pic2img.h"
#include "tmath.h"

float hsv_gray(Px_rgba px);

int main(int argc, char **argv)
{
	/* in1(qoi rgb)			->	out1(pgm gray) 
	 * in2(pgm gray > 255)	->	out2(qoi gray) */

	Image		 *pic1 = NULL;
	Image		 *pic2 = NULL;
	Pgm			  *in2 = NULL;
	Pgm			 *out1 = NULL;
	Qoimg	  *in1_rgb = NULL;
	Qoimg	*out2_gray = NULL;

	if (argc != 6) {
		fprintf(stderr, "argument error\n\n");
		fprintf(stderr, "Usage: %s <input file1 qoi rgb> <input file2 pgm > 255>"
						"<output file1 qoi> <output file2 pgm> <gray scale method>\n\n"
						"gray scale method: 709 or hsv\n", argv[0]);
		return __LINE__;
	}

	if ((pic1 = img_new(1, 1, 32, SRGB)) == NULL) {
		fprintf(stderr, "error can't new a Image object\n");
		return __LINE__;
	}
	if ((pic2 = img_new(IMG_EMPTY)) == NULL) {
		fprintf(stderr, "error can't new a Image object\n");
		return __LINE__;
	}

	if ((in2 = pgm_read(argv[2])) == NULL) {
		fprintf(stderr, "error reading PGM file %s fail\n", argv[2]);
		perror("");
		return __LINE__;
	}
	if (pgm_info(in2, stdout) != 0)
		return __LINE__;

	if ((out1 = pgm_new(PGM_EMPTY)) == NULL) {
		fprintf(stderr, "error can't new a Pgm object\n");
		return __LINE__;
	}
	if (pgm_set(out1, 3840, 2160, PGM_GRAY_16BIT) == NULL) {
		fprintf(stderr, "error can not reset Pgm object\n");
		return __LINE__;
	}

	if ((in1_rgb = qoi_read(argv[1])) == NULL) {
		fprintf(stderr, "error reading QOI file %s fail\n", argv[1]);
		perror("");
		return __LINE__;
	}
	if ((out2_gray = qoi_new(1920, 1080, QOI_CH_RGBA, QOI_GAMUT_SRGB_LA)) == NULL) {
		fprintf(stderr, "error can't create a Qoimg object\n");
		return __LINE__;
	}

	if (qoi2img(in1_rgb, pic1) == NULL) {
		fprintf(stderr, "error convert Qoimg to Image fail\n");
		return __LINE__;
	}
	if (pgm2img(in2, pic2) == NULL) {
		fprintf(stderr, "error convert Pgm to Image fail\n");
		return __LINE__;
	}

	if (img2pgm(pic1, out1, (strcmp(argv[5], "hsv") == 0) ? hsv_gray : NULL) == NULL) {
		fprintf(stderr, "error convert Image to Pgm fail\n");
		return __LINE__;
	}
	if (img2qoi(pic2, out2_gray) == NULL) {
		fprintf(stderr, "error convert Image to Qoimg fail\n");
		return __LINE__;
	}

	if (pgm_write(out1, argv[3]) != 0) {
		fprintf(stderr, "error fail to write a pgm file\n");
		return __LINE__;
	}
	if (qoi_write(out2_gray, argv[4]) != 0) {
		fprintf(stderr, "error fail to write a qoi file\n");
		return __LINE__;
	}

	if (img_free(pic1) != 0 || img_free(pic2) != 0)
		return __LINE__;
	if (pgm_free(out1) != 0 || pgm_free(in2) != 0)
		return __LINE__;
	if (qoi_free(in1_rgb) != 0 || qoi_free(out2_gray) != 0)
		return __LINE__;

	return 0;
}

float hsv_gray(Px_rgba px)
{
	return max3(px.r, px.g, px.b);
}