// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "chart.h"
#include "qoimg.h"
#include "pic2img.h"

int main(int argc, char *argv[])
{
	Qoimg *pic = NULL;
	Image *tmp = NULL;
	Chart *out = NULL;
	int col_unit = 80;
	double wid_rate = 1;

	if (argc < 2 ||  strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "Usage: %s <qoi image> [term cols] [wid rate]\n", argv[0]);
	}
	if (argc >= 3)
		col_unit = atoi(argv[2]);
	if (argc == 4)
		wid_rate = atof(argv[3]);
	
	if ((pic = qoi_read(argv[1])) == NULL) {
		fprintf(stderr, "error read qoi file %s fail", argv[1]);
		perror("");
		return __LINE__;
	}
	if ((tmp = img_new(IMG_EMPTY)) == NULL) {
		fprintf(stderr, "error can't new a Image object\n");
		return __LINE__;
	}

	if ((out = ca_new(CA_EMPTY)) == NULL) {
		fprintf(stderr, "error fail to new a Chart object\n");
		return __LINE__;
	}
	if (ca_set(out, 13, 13, col_unit) == NULL) {
		fprintf(stderr, "error can not reset a Chart\n");
		return __LINE__;
	}
	
	if (qoi2img(pic, tmp) == NULL) {
		fprintf(stderr, "error convert Qoimg to Image fail\n");
		return __LINE__;
	}
	if (img2ca(tmp, out) == NULL) {
		fprintf(stderr, "error convert Image to Chart fail\n");
		return __LINE__;
	}
	if (ca_info(out, stdout) != 0)
		return __LINE__;

	fprintf(stdout, "Press q to continue, k or j to scroll up or down\n");
	if (ca_write(out, wid_rate, stdout) != 0) {
		fprintf(stderr, "error fail to output a ASCII Art\n");
		return __LINE__;
	}

	if (img_free(tmp) != 0)
		return __LINE__;
	if (qoi_free(pic) != 0)
		return __LINE__;
	if (ca_free(out) != 0)
		return __LINE__;

	return 0;
}