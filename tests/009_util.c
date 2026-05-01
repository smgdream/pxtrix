// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#include <stdio.h>
#include "image.h"
#include "qoimg.h"
#include "pic2img.h"
#include "render.h"
#include "util.h"

static float gris(Px_def px)
{
	/* ITU-R BT.709-6 */
	return 0.2126 / 1.005f * px.r
		 + 0.7152 / 1.005f * px.g
		 + 0.0722 / 1.005f * px.b;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s OUTFILE\n", argv[0]);
		return __LINE__;
	}

	uint32_t x = 0, y = 0;
	Qoimg *io = NULL;
	Image *paper = NULL;
	Image *p1 = NULL;
	Image *p2 = NULL;
	Image *p3 = NULL;
	Image *p4 = NULL;

	if ((paper = img_new(IMG_EMPTY)) == NULL
	 || (p1 = img_new(IMG_EMPTY)) == NULL
	 || (p2 = img_new(IMG_EMPTY)) == NULL
	 || (p3 = img_new(IMG_EMPTY)) == NULL
	 || (p4 = img_new(IMG_EMPTY)) == NULL) {
		fprintf(stderr, "error new Image object fali\n");
		return __LINE__;
	 }
	
#if __STDC_VERSION__ < 202311L
	fprintf(stderr, "%s Note: please using C compiler support for "
					"ISO C23 standard for a complete test\n", __FILE__);
	if ((io = qoi_new(1920, 1080, QOI_CH_RGBA, QOI_GAMUT_SRGB_LA)) == NULL) {
		fprintf(stderr, "error fail to new a Qoimg object\n");
	}
#else
	static const uint8_t inside_file[] = {
		#embed "pictures/util_src_img.qoi"
	};
	FILE *fp = NULL;
	if ((fp = byteseq2tmpfile(inside_file, sizeof inside_file)) == NULL) {
		fprintf(stderr, "error write file from byte sequence fail\n");
		perror("");
		return __LINE__;
	}
	if ((io = qoi_read_stream(fp)) == NULL) {
		fprintf(stderr, "error read QOI file from stream fail\n");
		return __LINE__;
	}
	fclose(fp);
#endif

	timer_start();
	time_step();

	if (qoi2img(io, paper) == NULL) {
		fprintf(stderr, "error fail to convert Qoimg to Image\n");
		return __LINE__;
	}
	printf("qoi2img: %.3fms\n", time_step() / 1000.0);
	
	/* Image copy and crop */
	if (img_copy(p1, paper) == NULL) {
		fprintf(stderr, "error Image copy fail\n");
		return __LINE__;
	}
	if (crop(p1, 0, p1->height/2, p1->width/2, p1->height/2) == NULL) {
		fprintf(stderr, "error crop fail\n");
		return __LINE__;
	}
	if (img_copy(p2, paper) == NULL) {
		fprintf(stderr, "error Image copy fail\n");
		return __LINE__;
	}
	if (crop(p2, p2->width/2, p2->height/2, p2->width/2, p2->height/2) == NULL) {
		fprintf(stderr, "error crop fail\n");
		return __LINE__;
	}
	if (img_copy(p3, paper) == NULL) {
		fprintf(stderr, "error Image copy fail\n");
		return __LINE__;
	}
	if (crop(p3, 0, 0, p3->width/2, p3->height/2) == NULL) {
		fprintf(stderr, "error crop fail\n");
		return __LINE__;
	}
	if (img_copy(p4, paper) == NULL) {
		fprintf(stderr, "error Image copy fail\n");
		return __LINE__;
	}
	if (crop(p4, p4->width/2, 0, p4->width/2, p4->height/2) == NULL) {
		fprintf(stderr, "error crop fail\n");
		return __LINE__;
	}
	printf("crop: %.3fms\n", time_step() / 1000.0);

	/* p1 process */
	img_for_px(x, y, 0, 0, p1->width, p1->height) {
		Px_rgba px = *img_px(p1, x, y);
		Px_rgba px1 = {0, 0, 0, px.a};
		double gray_val = gris(px);

		px1.r = lerp(px.r, gray_val, (double)x / (p1->width-1));
		px1.g = lerp(px.g, gray_val, (double)x / (p1->width-1));
		px1.b = lerp(px.b, gray_val, (double)x / (p1->width-1));
		*img_px(p1, x, y) = px1;
	}
	printf("p1 proc: %.3fms\n", time_step() / 1000.0);
	
	/* p2 process */
	if (flip_v(flip_h(p2)) == NULL) {
		fprintf(stderr, "error any error occurs when process p2\n");
		return __LINE__;
	}
	printf("p2 proc: %.3fms\n", time_step() / 1000.0);

	/* p3 process */
	if (invert(p3) == NULL) {
		fprintf(stderr, "error invert p3 error\n");
		return __LINE__;
	}
	img_for_px(x, y, 0, 0, p3->width, p3->height) {
		Px_rgba px = *img_px(p3, x, y);
		px.a = 0.666f;
		*img_px(p3, x, y) = px;
	}
	printf("p3 proc: %.3fms\n", time_step() / 1000.0);

	/* p4 process */
	if (gray(p4, NULL) == NULL) {
		fprintf(stderr, "error p4 gray scale fail\n");
		return __LINE__;
	}
	printf("p4 proc: %.3fms\n", time_step() / 1000.0);

	// [1][2] -> [3][4]
	// [3][4] -> [2][1]
	if (overlay(paper, p1, paper->width/2, 0) == NULL
	 || overlay(paper, p2, 0, 0) == NULL
	 || overlay(paper, p3, 0, paper->height/2) == NULL
	 || overlay(paper, p4, paper->width/2, paper->height/2) == NULL) {
		fprintf(stderr, "error any error occurs when overlay\n");
		return __LINE__;
	}
	printf("Time all after overlay: %.3fms\n", time_all() / 1000.0);

	if (img_free(p1) != 0 
	 || img_free(p2) != 0
	 || img_free(p3) != 0
	 || img_free(p4) != 0)
		return __LINE__;

	timer_reset();
	if (img2qoi(paper, io) == NULL) {
		fprintf(stderr, "error Image to Qoimg fail\n");
		return __LINE__;
	}
	printf("img2qoi: %.3fms\n", time_step() / 1000.0);
	if (qoi_write(io, argv[1]) != 0) {
		fprintf(stderr, "error fail to write Qoimg to QOI file\n");
		return __LINE__;
	}
	printf("qoi_write: %.3fms\n", time_step() / 1000.0);

	if (img_free(paper) != 0)
		return __LINE__;
	if (qoi_free(io) != 0)
		return __LINE__;
	msleep(3000);
	return 0;
}