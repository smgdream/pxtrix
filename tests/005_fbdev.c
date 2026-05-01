// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "image.h"
#include "fbdev.h"
#include "qoimg.h"
#include "pic2img.h"
#include "util.h"


#ifdef __unix__
static int fexist(const char *filename)
{
	if (filename == NULL)
		return 0;

	FILE *fp = fopen(filename, "rb");
	if (fp == NULL)
		return 0;
	fclose(fp);
	return 1;
}
static const char *fbpath = "/dev/fb0";

int main(int argc, char *argv[])
{
	/* if argv[1] == "ss" // tty screenshot
	 *     in0 -> tmp -> out0
	 * else // FBI a.k.a. Framebuffer Image viewer (crop ver.)
	 *     in1 -> tmp -> out1 */
	Image *tmp	= NULL;
	Qoimg *in1	= NULL;
	Qoimg *out0	= NULL;
	Fbdev *in0	= NULL;
	Fbdev *out1	= NULL;

	if (argc < 3) {
		fprintf(stderr, "error: too few arguments\n\n"
						"Usage: %s ss <tty screenshot save file name.qoi>\n"
						"\tOR\n"
						"       %s qv <filename.qoi>\n", argv[0], argv[0]);
		return __LINE__;
	}

	if (!fexist(fbpath)) {
		fprintf(stderr, "%s warning: %s is not exist\n", __FILE__, fbpath);
		return 0;
	}

	if ((tmp = img_new(IMG_EMPTY)) == NULL) {
		fprintf(stderr, "error can't new a Image object\n");
		return __LINE__;
	}

	if (strcmp(argv[1], "ss") == 0) {
		if ((in0 = fb_read(fbpath)) == NULL) {
			fprintf(stderr, "error read framebuffer device %s fail\n", fbpath);
			perror("");
			return __LINE__;
		}
		if (fb_info(in0, stdout) != 0)
			return __LINE__;
		if ((out0 = qoi_new(QOI_EMPTY)) == NULL) {
			fprintf(stderr, "error can't new a Qoimg object\n");
			return __LINE__;
		}

		if (fb2img(in0, tmp) == NULL) {
			fprintf(stderr, "error convert Fbdev to Image fail\n");
			return __LINE__;
		}
		if (img2qoi(tmp, out0) == NULL) {
			fprintf(stderr, "error convert Image to Qoimg fail\n");
			return __LINE__;
		}

		if (qoi_write(out0, argv[2]) != 0) {
			fprintf(stderr, "error fail to write a QOI file\n");
			return __LINE__;
		}

		if (fb_free(in0) != 0)
			return __LINE__;
		if (qoi_free(out0) != 0)
			return __LINE__;
	} else {
		uint32_t wid = 0, hgt = 0;

		if ((out1 = fb_new(3, 3, FB_FILE)) == NULL) {
			fprintf(stderr, "error can not new a Fbdev object\n");
			return __LINE__;
		}
		if (fb_display_info(fbpath, &wid, &hgt) != 0) {
			fprintf(stderr, "error can not get framebuffer display info: %s\n", fbpath);
			return __LINE__;
		}
		if (fb_set(out1, wid, hgt, FB_DEVICE) == NULL) {
			fprintf(stderr, "error reset Fbdev object fail\n");
			return __LINE__;
		}
		
		if ((in1 = qoi_read(argv[2])) == NULL) {
			fprintf(stderr, "error read qoi file %s fail", argv[1]);
			perror("");
			return __LINE__;
		}

		if (qoi2img(in1, tmp) == NULL) {
			fprintf(stderr, "error convert Qoimg to Image fail\n");
			return __LINE__;
		}
		if (img2fb(tmp, out1) == NULL) {
			fprintf(stderr, "error convert Image to Fbdev fail\n");
			return __LINE__;
		}
		
		if (tolower(argv[1][0]) > 'm') {
			FILE *fp = fopen(fbpath, "wb");

			if (fp == NULL) {
				fprintf(stderr, "error fail to open %s\n", fbpath);
				return __LINE__;
			}
			if (fb_write_stream(out1, fp) != 0) {
				fprintf(stderr, "error write framebuffer stream fail\n");
				fclose(fp);
				return __LINE__;
			}
			fclose(fp);
		} else {
			if (fb_write(out1, fbpath) != 0) {
				fprintf(stderr, "error fail to write framebuffer device: %s\n", fbpath);
				return __LINE__;
			}
		}
		
		if (fb_free(out1) != 0)
			return __LINE__;
		if (qoi_free(in1) != 0)
			return __LINE__;
	}
	
	if (img_free(tmp) != 0)
		return __LINE__;
	return 0;
}
#else
int main(void)
{
	fprintf(stderr, "%s only support for UNIX-like system\n", __FILE__);
	return 0;
}
#endif