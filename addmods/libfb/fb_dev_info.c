// SPDX-License-Identifier: MIT
/* Copyright (c) 2025 Smgdream */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fbdev.h"

#ifdef __linux__
	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/ioctl.h>
	#include <linux/fb.h>
#endif

static size_t _fb_file_size(const char *name)
{
	size_t siz = 0;
	char tmp = '\0';
	FILE *fp = NULL;

	if ((fp = fopen(name, "rb")) == NULL)
		return 0;
	for (; fread(&tmp, 1, 1, fp) == 1; ++siz)
		;

	fclose(fp);
	return siz;
}

#ifdef __linux__
int fb_display_info(const char *name, uint32_t *xres, uint32_t *yres)
{
	struct fb_var_screeninfo fb_info;
	int fd = -1;

	if (name == NULL || xres == NULL || yres == NULL)
		return -1;
	if ((fd = open(name, O_RDONLY)) == -1) {
		fprintf(stderr, "Error open \"%s\" fail: ", name);
		perror("");
		*xres = *yres = 0;
		return -2;
	}

	if (ioctl(fd, FBIOGET_VSCREENINFO, &fb_info) == -1) {
		fprintf(stderr, "Error can't get the display info of %s\n", name);
		return -3;
	}
	*xres = fb_info.xres;
	*yres = fb_info.yres;

	if (close(fd) != 0) {
		fprintf(stderr, "Warning can't close \"%s\": ", name);
		perror("");
		return -4;
	}
	return 0;
}
#endif

struct fb_base_info _fb_dev_info(const char *name)
{
	size_t siz = 0;
	struct fb_base_info info = {0, 0};

	if (name == NULL)
		return info;

	/* get the screen resolution from framebuffer device */
#ifdef __linux__
	if (strncmp(name, "/dev/fb", 7) == 0) {
		fb_display_info(name, &info.xres, &info.yres);
		return info;
	}
#endif

	/* calculating the resolution of framebuffer cache file */
	siz = _fb_file_size(name);
	if (info.xres * info.yres != siz) {
		/* Is not frame buffer device is fream buffer save */
		switch (siz) {
		case 4096*2160: /* DCI 4K */
			info.xres = 4096;
			info.yres = 2160;
			break;
		case 3840*2160: /* UHD 4K */
			info.xres = 3840;
			info.yres = 2160;
			break;
		case 3000*2000:
			info.xres = 3000;
			info.yres = 2000;
			break;
		case 2560*1440: /* 2K */
			info.xres = 2560;
			info.yres = 1440;
			break;
		case 1920*1200: /* pro display */
			info.xres = 1920;
			info.yres = 1200;
			break;
		case 1920*1080: /* 1080P */
			info.xres = 1920;
			info.yres = 1080;
			break;
		case 1366*768: /* Old-laptop display */
			info.xres = 1366;
			info.yres = 768;
			break;
		case 1280*1024: /* square display */
			info.xres = 1280;
			info.yres = 1024;
			break;
		case 1280*720: /* 720P */
			info.xres = 1280;
			info.yres = 720;
			break;
		case 1024*768: /* Old display */
			info.xres = 1024;
			info.yres = 768;
			break;
		case 800*600: /* OO display */
			info.xres = 800;
			info.yres = 600;
			break;
		case 640*480: /* 480P */
			info.xres = 480;
			info.yres = 640;
			break;
		default:
			fprintf(stderr, "%s: unknow image size: %zu\n", __func__, siz);
				return info;
			break;
		}
	}
	return info;
}
