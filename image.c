/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "image.h"

Image *img_new(uint32_t wid, uint32_t hgt, uint32_t bpp, uint32_t gamut)
{
	Image *img = NULL;

	if ( (img = (Image *) malloc(sizeof(Image))) == NULL )
		return NULL;
	
	img->buf = NULL;
	/* Set {r: 0, g: 0, b: 0 a: 0} as defult color */
	if ( wid * hgt != 0
	  && (img->buf = (Px_def *) calloc(1, wid * hgt * sizeof(Px_def))) == NULL ) {
		/* need to alloc a memory area but fail */
		free(img);
		return NULL;
	}

	img->width = wid;
	img->height = hgt;
	img->bpp = bpp;
	img->gamut = gamut;
	return img;
}

int img_free(Image *img)
{
	if (img == NULL)
		return -1;
	if (img->buf != NULL)
		free(img->buf);
	free(img);
	return 0;
}

Image *img_set(Image *img, uint32_t wid, uint32_t hgt, uint32_t bpp, uint32_t gamut)
{
	if (img == NULL)
		return NULL;

	/* set a blank image */
	if (wid * hgt == 0 && img->buf != NULL) {
		free(img->buf);
		img->buf = NULL;
	}

	/* Reallocate the pixel buffer */
	if (img->width * img->height != wid * hgt && wid * hgt != 0) {
		Px_def *new_buf = NULL;

		/* Try to alloc the new buffer, if fail retain the old Image object */
		/* Set {r: 0, g: 0, b: 0 a: 0} as defult color */
		if ((new_buf = (Px_def *) calloc(1, wid * hgt * sizeof(Px_def))) == NULL )
			return NULL;
		if (img->buf != NULL)
			free(img->buf);
		img->buf = new_buf;
	}

	img->width = wid;
	img->height = hgt;
	img->bpp = bpp;
	img->gamut = gamut;
	return img;		
}

Image *img_copy(Image *dest, const Image *src)
{
	uint32_t x = 0, y = 0;
	
	if (src == NULL || dest == NULL)
		return NULL;
	if ( dest->width != src->width
	  || dest->height != src->height
	  || dest->bpp != src->bpp
	  || dest->gamut != src->gamut )
		if (img_set(dest, src->width, src->height, src->bpp, src->gamut) == NULL)
			return NULL;

	img_for_px(x, y, 0, 0, src->width, src->height)
		*img_px(dest, x, y) = *img_px(src, x, y);
	return dest;
}

int img_info(const Image *img, void *fptr)
{
	FILE *fp = (FILE *)fptr;

	if (img == NULL || fp == NULL)
		return 1;

	fprintf(fp, "%s:\t%u\n", "width",	img->width);
	fprintf(fp, "%s:\t%u\n", "height",	img->height);
	fprintf(fp, "%s:\t%u\n", "bbp",		img->bpp);
	fprintf(fp, "%s:\t%u\n", "gamut",	img->gamut);

	return 0;
}