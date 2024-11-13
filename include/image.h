/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include "pixel.h"

enum color_gamut { sRGB, DCI_P3, Adobe_RGB };

typedef struct image {
	uint32_t width;	 // image width in pixels
	uint32_t height; // image height in pixels
	uint32_t bpp;	 // bits per pixel
	uint32_t gamut;	 // color gamut
	Px_def	*buf;	 // pixel buffer
} Image;

/* New a Image object. Returns pointer to it, or NULL if any error occurs.  */
Image *img_new(uint32_t wid, uint32_t hgt, uint32_t bpp, uint32_t gamut);
/* Free the Image object img. Returns non-zero on error.  */
int img_free(Image *img);
/* Reconfigure the Image object, Returns img, or NULL if any error occurs.  */
Image *img_set(Image *img, uint32_t wid, uint32_t hgt, uint32_t bpp, uint32_t gamut);
/* Copy a Image object from src to dest. Returns dest, or NULL if any error occurs.  */
Image *img_copy(Image *dest, const Image *src);

/* img_px using define in tcc, or inline function in other compiler */
/* Get the pixel at (x, y) from img,
 * return the pointer to it, or NULL if some error occurs.  */
#if __TINYC__
	#define img_px(img, x, y) ( (Px_def *) ( \
		(img == NULL) ? \
			NULL : \
		(x >= (img)->width || y >= (img)->height) ? \
			NULL : \
			&(img)->buf[ (x) + (y) * (img)->width ] \
	) )
#else
	static inline Px_def *img_px(const Image *img, uint32_t x, uint32_t y)
	{
		if (img == NULL || x >= img->width || y >= img->height)
			return NULL;
		return &img->buf[x + y * img->width];
	}
#endif

/* For each position by iterator x_name and y_name,  
 * from (x_beg, y_beg) to (x_beg+wid-1, y_beg+hgt-1).
 * Not recommend using the expression with side effect.  */
#define img_for_px(x_name, y_name, x_beg, y_beg, wid, hgt) \
	for (y_name = (y_beg); y_name < (y_beg)+(hgt); ++y_name) \
		for (x_name = (x_beg); x < (x_beg)+(wid); ++x_name)

#define IMG_EMPTY 0, 0, 32, sRGB

#endif
