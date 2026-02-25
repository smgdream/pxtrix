/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fbdev.h"

static size_t __fb_file_size(const char *name)
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

struct fb_base_info __fb_dev_info(const char *name)
{
	FILE *fp = NULL;
	size_t siz = 0;
	struct fb_base_info info = { 0, 0 };

	system("fbset | head -n 3 | grep \"mode\" > fb_info.tmp");
	if ((fp = fopen("fb_info.tmp", "r")) == NULL) {
		remove("fb_info.tmp");
		return info;
	}
	fscanf(fp, "mode \"%ux%u\"", &info.xres, &info.yres);
	fclose(fp);
	remove("fb_info.tmp");

	if (strncmp(name, "/dev/fb", 7) == 0)
		return info;

	siz = __fb_file_size(name);
	if (info.xres * info.yres != siz) {
		/* Is not frame buffer device is fream buffer save */
		switch (siz) {
		case 4096*2160:
			info.xres = 4096;
			info.yres = 2160;
			break;
		case 3840*2160:
			info.xres = 3840;
			info.yres = 2160;
			break;
		case 3000*2000:
			info.xres = 3000;
			info.yres = 2000;
			break;
		case 2560*1440:
			info.xres = 2560;
			info.yres = 1440;
			break;
		case 1920*1200:
			info.xres = 1920;
			info.yres = 1200;
			break;
		case 1920*1080:
			info.xres = 1920;
			info.yres = 1080;
			break;
		case 1366*768:
			info.xres = 1366;
			info.yres = 768;
			break;
		case 1280*1024:
			info.xres = 1280;
			info.yres = 1024;
			break;
		case 1024*768:
			info.xres = 1024;
			info.yres = 768;
			break;
		case 800*600:
			info.xres = 800;
			info.yres = 600;
			break;
		default:
			fprintf(stderr, "%s: unknow image size: %zu\n", __func__, siz);
				return info;
			break;
		}
	}
	return info;
}
