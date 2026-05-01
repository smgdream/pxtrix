// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#include <stdio.h>
#include "image.h"
#include "qoimg.h"
#include "pic2img.h"

int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s IN_QOI OUT_QOI\n", argv[0]);
		return __LINE__;
	}

	Qoimg *io = NULL;
	Image *pic = NULL;

	if ((io = qoi_read(argv[1])) == NULL) {
		fprintf(stderr, "error fail to read the QOI file %s\n", argv[1]);
		perror("");
		return __LINE__;
	}
	if ((pic = img_new(IMG_EMPTY)) == NULL) {
		fprintf(stderr, "error fail to new a Image object\n");
		return __LINE__;
	}
	if (qoi2img(io, pic) == NULL) {
		fprintf(stderr, "error covert Qoimg to Image fail\n");
		return __LINE__;
	}

	if (img_copy(pic, pic) == NULL) {
		fprintf(stderr, "error copy Image fail\n");
		return __LINE__;
	}

	if(img2qoi(pic, io) == NULL) {
		fprintf(stderr, "error covert Image to Qoimg fail\n");
		return __LINE__;
	}
	if (qoi_write(io, argv[2]) != 0) {
		fprintf(stderr, "error write QOI file %s fail\n", argv[2]);
		perror("");
		return __LINE__;
	}

	if (qoi_free(io) != 0)
		return __LINE__;
	if (img_free(pic) != 0)
		return __LINE__;
	return 0;
}