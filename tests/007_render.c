// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#include <stdio.h>
#include <string.h>
#include "image.h"
#include "qoimg.h"
#include "pic2img.h"
#include "render.h"

static float gris(Px_def px)
{
	return max3(px.r, px.g, px.b);
}

int main(int argc, char *argv[])
{
	// mode0 overlay: (new0.fill() += in1.gray().scale(up).crop().invert()
	//							   +=  in2a.scale(down).flip_v().flip_h()
	// mode1 draw: new0.fill().line_row().line_col().beizer_curve()
	// mode1 bin: in1.bin()
	if (argc < 3) {
		fprintf(stderr, "%s mode0 <in file0> <in file 1> <out file>\n", argv[0]);
		fprintf(stderr, "%s mode1 <out file>\n", argv[0]);
		fprintf(stderr, "%s mode2 <in file0> <out file>\n", argv[0]);
		return 0;
	}

	Image *img0 = NULL; // new0
	Image *img1 = NULL; // in1
	Image *img2 = NULL; // in2
	Qoimg *pic1 = NULL; // in1_raw
	Qoimg *pic2 = NULL; // in2_raw
	Qoimg *out  = NULL;

	if ((img0 = img_new(3840, 2160, 32, SRGB)) == NULL) {
		fprintf(stderr, "error fail to new a Image object\n");
		return __LINE__;
	}
	if ((out = qoi_new(QOI_EMPTY)) == NULL) {
		fprintf(stderr, "error fail to new a Qoimg object\n");
		return __LINE__;
	}

	if (strcmp(argv[1], "mode0") == 0) {
		if ((img1 = img_new(IMG_EMPTY)) == NULL) {
			fprintf(stderr, "error new a Image object fail\n");
			return __LINE__;
		}
		if ((img2 = img_new(IMG_EMPTY)) == NULL) {
			fprintf(stderr, "error new a Image object fail\n");
			return __LINE__;
		}
		
		if ((pic1 = qoi_read(argv[2])) == NULL) {
			fprintf(stderr, "error fail to read QOI file %s\n", argv[2]);
			perror("");
			return __LINE__;
		}
		if ((pic2 = qoi_read(argv[3])) == NULL) {
			fprintf(stderr, "error fail to read QOI file %s\n", argv[3]);
			perror("");
			return __LINE__;
		}

		if (qoi2img(pic1, img1) == NULL) {
			fprintf(stderr, "error convert Qoimg to Image fail\n");
			return __LINE__;
		}
		if (qoi2img(pic2, img2) == NULL) {
			fprintf(stderr, "error convert Qoimg to Image fail\n");
			return __LINE__;
		}
		
		// in1 proc
		if (gray(img1, NULL) == NULL)
			return __LINE__;
		if (scale(img1, 1.5*img1->width, 1.5*img1->height) == NULL)
			return __LINE__;
		if (crop(img1, 0.5/3.0*img1->width, 0.01/3.0*img1->height, 
					   2.5/3.0*img1->width, 2.99/3.0*img1->height) == NULL)
			return __LINE__;
		if (invert(img1) == NULL)
			return __LINE__;
		
		// in2 proc
		if (scale(img2, 0.99 * img2->width, 0.99 * img2->height) == NULL)
			return __LINE__;
		if (flip_v(img2) == NULL)
			return __LINE__;
		if (flip_h(img2) == NULL)
			return __LINE__;
		
		if (fill(img0, rgba_hex(0xffffffff)) == NULL)
			return __LINE__;
		if (overlay(img0, img1, 39, 39) == NULL)
			return __LINE__;

		if (overlay(img0, img2, ((int64_t)img1->width - img2->width)/2,
								((int64_t)img1->height - img2->height)/2) == NULL)
			return __LINE__;

		if (img2qoi(img0, out) == NULL) {
			fprintf(stderr, "error convert Image to Qoimg fail\n");
			return __LINE__;
		}

		if (qoi_free(pic1) != 0 || qoi_free(pic2) != 0)
			return __LINE__;
		if (img_free(img1) != 0 || img_free(img2) != 0)
			return __LINE__;
	} else if (strcmp(argv[1], "mode1") == 0) {
		if (fill(img0, rgba_4val(225, 225, 225, 255)) == NULL)
			return __LINE__;
		for (int i = 0; i < 10; ++i) {
			if (line_col(img0, i/10.0*img0->width, 2, rgba_hex(0x101010ff)) == NULL)
				return __LINE__;
			if (line_row(img0, i/10.0*img0->height, 2, rgba_hex(0x000000ff)) == NULL)
				return __LINE__;
		}
		if (bezier_curve(img0, (Vec2){2600, 600},
							   (Vec2){1600, 1300},
							   (Vec2){2300, 1300},
							   (Vec2){1300, 600},
							   rgba_hex(0x00ffccf0)) == NULL)
			return __LINE__;

		if (img2qoi(img0, out) == NULL) {
			fprintf(stderr, "error convert Image to Qoimg fail\n");
			return __LINE__;
		}
	} else if (strcmp(argv[1], "mode2") == 0) {
		if ((img1 = img_new(IMG_EMPTY)) == NULL) {
			fprintf(stderr, "error new a Image object fail\n");
			return __LINE__;
		}

		if ((pic1 = qoi_read(argv[2])) == NULL) {
			fprintf(stderr, "error fail to read QOI file %s\n", argv[2]);
			perror("");
			return __LINE__;
		}
		if (qoi2img(pic1, img1) == NULL) {
			fprintf(stderr, "error convert Qoimg to Image fail\n");
			return __LINE__;
		}
		if (bin(img1, gris, 0.5) == NULL)
			return __LINE__;

		if (img2qoi(img1, out) == NULL) {
			fprintf(stderr, "error convert Image to Qoimg fail\n");
			return __LINE__;
		}
		if (img_free(img1) != 0 )
			return __LINE__;
	} else {
		fprintf(stderr, "error unknow mode: %s\n", argv[1]);
		return __LINE__;
	}

	if (qoi_write(out, argv[argc-1]) != 0) {
		fprintf(stderr, "error fail to write the QOI file %s\n", argv[argc-1]);
		perror("");
		return __LINE__;
	}
	
	if (qoi_free(out) != 0)
		return __LINE__;
	if (img_free(img0) != 0)
		return __LINE__;
	return 0;
}