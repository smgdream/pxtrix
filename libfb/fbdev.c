/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include "fbdev.h"


struct fb_base_info __fb_dev_info(const char *filename);

Fbdev *fb_new(uint32_t xres, uint32_t yres, uint8_t dev_flag)
{
	Fbdev *tmp = NULL;

	if ((tmp = malloc(sizeof(Fbdev))) == NULL)
		return NULL;

	tmp->buf = NULL;
	if ( xres * yres != 0
	 && (tmp->buf = calloc(1, xres * yres * sizeof(Fb_px_def))) == NULL) {
		free(tmp);
		return NULL;
	}

	tmp->xres = xres;
	tmp->yres = yres;
	tmp->dev_flag = dev_flag;
	return tmp;
}

int fb_free(Fbdev *fbd)
{
	if (fbd == NULL)
		return 1;

	if (fbd->buf != NULL)
		free(fbd->buf);
	free(fbd);

	return 0;
}

Fbdev *fb_read(const char *name)
{	
	Fbdev *tmp = NULL;
	FILE *fp  = NULL;

	if (!fb_valid(name) || (fp = fopen(name, "rb")) == NULL)
		return NULL;

	struct fb_base_info info = __fb_dev_info(name);
	tmp = fb_new(info.xres, info.yres, 
				(strncmp(name, "/dev/fb", 7) == 0) ? 1 : 0
			);
	if (tmp == NULL) {
		fclose(fp);
		return NULL;
	}

	uint64_t cnt = fread(tmp->buf, tmp->xres * tmp->yres * sizeof(Fb_px_def), 1, fp);
	if (cnt != 1) {
		fprintf(stderr, "%s: size error\n", __func__);
		fb_free(tmp);
		fclose(fp);
		return NULL;
	}

	fclose(fp);
	return tmp;
}

int fb_write(const Fbdev *fbd, const char *name)
{
	FILE *fp = NULL;

	if (fbd == NULL || fbd->buf == NULL || (fp = fopen(name, "wb")) == NULL)
		return -1;

	fwrite(fbd->buf, fbd->xres * fbd->yres * sizeof(Fb_px_def), 1, fp);

	fclose(fp);
	return 0;
}

int fb_info(const Fbdev *fbd, void *fptr)
{
	FILE *fp = (FILE *) fptr;

	if (fbd == NULL || fptr == NULL)
		return -1;
		
	fprintf(fp, "%s:\t%"PRIu32"\n", "xres", fbd->xres);
	fprintf(fp, "%s:\t%"PRIu32"\n", "yres", fbd->yres);
	fprintf(fp, "%s:\t%"PRIu8 " (%s)\n", "device flag", fbd->dev_flag,
		(fbd->dev_flag == FB_FILE) ? "framebuffer saving" : "framebuffer device");
	return 0;
}

Fbdev *fb_set(Fbdev *fbd, uint32_t xres, uint32_t yres, uint8_t dev_flag)
{
	if (fbd == NULL)
		return NULL;

	/* if xres * yres == 0 free the buffer */
	if (xres * yres == 0 && fbd->buf != NULL) {
		free(fbd->buf);
		fbd->buf = NULL;
	}

	/* reallocation the buffer */
	if (xres * yres != 0 && xres * yres != fbd->xres * fbd->yres) {
		Fb_px_def *new_buf = NULL;

		if ((new_buf = calloc(1, xres * yres * sizeof(Fb_px_def))) == NULL)
			return NULL;
		if (fbd->buf != NULL)
			free(fbd->buf);
		fbd->buf = new_buf;
	}

	fbd->xres = xres;
	fbd->yres = yres;
	fbd->dev_flag = dev_flag;

	return fbd;
}

int fb_valid(const char *name)
{
	FILE *fp = NULL;

	if (name == NULL || (fp = fopen(name, "rb")) == NULL)
		return 0; // invalid fbdev
	else {
		fclose(fp);
		return 1; // valid fbdev
	}
}
