// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#include <stdio.h>
#include "image.h"
#include "qoimg.h"
#include "pic2img.h"

int main(int argc, char *argv[])
{
	Image *img = NULL;
	Qoimg *in = NULL;
	Qoimg *out = NULL;

	if (argc != 3) {
		fprintf(stderr, "too few argument\n\n%s <input file> <output file>\n", argv[0]);
		return __LINE__;
	}
	
	if (argv[1][0] > 'm') {
		FILE *fp = NULL;

		if ((fp = fopen(argv[1], "rb")) == NULL) {
			fprintf(stderr, "error open file %s fail", argv[1]);
			perror("");
			return __LINE__;
		}
		if ((in = qoi_read_stream(fp)) == NULL) {
			fprintf(stderr, "error read stream error (file: %s)", argv[1]);
			perror("");
			fclose(fp);
			return __LINE__;
		}
		fclose(fp);
	} else {
		if ((in = qoi_read(argv[1])) == NULL) {
			fprintf(stderr, "error read qoi file %s fail", argv[1]);
			perror("");
			return __LINE__;
		}
	}
	if (qoi_info(in, stdout) != 0)
		return __LINE__;

	if ((out = qoi_new('f'+'o'+'r', 'n'+'a', QOI_CH_RGB, QOI_GAMUT_SRGB_LA)) == NULL) {
		fprintf(stderr, "error can't new a Qoimg object\n");
		return __LINE__;
	}
	if (qoi_set(out,
				out->header.width, 
				out->header.height, 
				QOI_CH_RGBA, 
				QOI_GAMUT_SRGB_LA) == NULL) {
		fprintf(stderr, "error fail to reset Qoimg\n");
		return __LINE__;
	}
	
	if ((img = img_new(IMG_EMPTY)) == NULL) {
		fprintf(stderr, "error can't new a Image object\n");
		return __LINE__;
	}

	if (qoi2img(in, img) == NULL) {
		fprintf(stderr, "error convert Qoimg to Image fail\n");
		return __LINE__;
	}
	if (img2qoi(img, out) == NULL) {
		fprintf(stderr, "error convert Imgae to Qoimg fail\n");
		return __LINE__;
	}

	if (qoi_write(out, argv[2]) != 0) {
		fprintf(stderr, "error fail to write a QOI file\n");
		return __LINE__;
	}

	if (img_free(img) != 0)
		return __LINE__;
	if (qoi_free(in) != 0 || qoi_free(out) != 0)
		return __LINE__;
	return 0;
}