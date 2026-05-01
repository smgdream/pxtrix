// SPDX-License-Identifier: MIT
/* Copyright (c) 2024 Smgdream */

/* 
 * Coordinate of Image
 *
 * y ^ 
 *   |
 *   |  img px
 *   |
 *   +----------> 
 * 0             x
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <stddef.h>
#include <stdint.h>
#include "pixel.h"

/* sRGB: IEC 61966-2-1
 * opRGB: IEC 61966-2-5 compatible with Adobe RGB 1998
 * BT.2020: ITU-R BT.2020-2
 * ACEScg: ACEScg Specification - ACEScg - A Working Space for CGI Render and
 * Compositing. Note: ACEScg is linear color space
 * UNO_LINEAR: Unkonwn color space with linear transfer function 
 * UNO_NON_LINEAR: Unkonwn color space with non-linear transfer function */

enum img_color_gamut { SRGB, OPRGB, BT_2020, ACESCG, UNO_NON_LINEAR, UNO_LINEAR };

typedef struct image {
	uint32_t width;	 // image width in pixels
	uint32_t height; // image height in pixels
	uint32_t bpp;	 // bits per pixel
	uint32_t gamut;	 // color gamut
	Px_def	*buf;	 // pixel buffer
} Image;


/* New a Image object. Returns pointer to it, or NULL if any error occurs.
 * The defaul color of image is #00000000.  */
Image *img_new(uint32_t wid, uint32_t hgt, uint32_t bpp, uint32_t gamut);
/* Free the Image object img. Returns non-zero on error.  */
int img_free(Image *img);
/* Reconfigure the Image object, Returns img, or NULL if any error occurs. 
 * If the size of new pixel buffer equal to the size of old buffer, it would 
 * not reallocate the pixel buffer, vice versa. If the width or the height of 
 * the image has change. The content of image is implementation-defined.  */
Image *img_set(Image *img, uint32_t wid, uint32_t hgt, uint32_t bpp, uint32_t gamut);
/* Copy a Image object from src to dest. Returns dest, or NULL if any error occurs.  */
Image *img_copy(Image *dest, const Image *src);

/* img_px defined as macro in tcc, or inline function in other compiler */
/* Get the pixel at (x, y) from img, returns the pointer to it, or NULL if some
 * error occurs. 
 * The argument type and return type is confusing. You just need to know it can
 * accept a Image * or a const Image * as argument. Then returns a Px_def *,
 * the Px_def object of return value pointer to is writable, whether the
 * argument you given is constent or not.  */
#ifndef __TINYC__
	static inline Px_def *img_px(const Image *img, uint32_t x, uint32_t y)
	{
		if (img == NULL || x >= img->width || y >= img->height)
			return NULL;
		return &img->buf[x + y * img->width];
	}
	
#else
	#define img_px(img, x, y) ( (Px_def *) ( \
		((img) == NULL) ? \
			NULL : \
		((x) >= (img)->width || (y) >= (img)->height) ? \
			NULL : \
			&(img)->buf[ (uint32_t) (x) + (uint32_t) (y) * (img)->width ] \
	) )
#endif

/* For each position by iterator x_name and y_name,  
 * from (x_beg, y_beg) to (x_beg+wid-1, y_beg+hgt-1).
 * Not recommend using the expression with side effect.  */
#define img_for_px(x_name, y_name, x_beg, y_beg, wid, hgt) \
	for ((y_name) = (y_beg); (y_name) < (y_beg)+(hgt); ++(y_name)) \
		for ((x_name) = (x_beg); (x_name) < (x_beg)+(wid); ++(x_name))

#define IMG_EMPTY 0, 0, 32, SRGB

/* Origin is in the center of the image.
 * vaild range: (-width/2, -height/2) -> (width/2-1, height/2-1).  */
#define img_px_c(img, x ,y) \
	img_px((img), \
		(uint32_t) ((x) + (int32_t) (img)->width / 2), \
		(uint32_t) ((y) + (int32_t) (img)->height / 2) \
	)

/* Px_def *img_px_v2(const Image *img, Vec2 vec) */
#define img_px_v2(img, vec) ( \
	img_px((img), (uint32_t) (vec).x, (uint32_t) (vec).y) \
)

/* Image object is empty or not.  */
static inline int img_is_empty(const Image *img)
{
	return img == NULL || img->buf == NULL
		|| img->width == 0 || img->height == 0;
}

/* Print the information of specific Image object to a file stream.
 * Returns 0, or non-zero on error.  */
int img_info(const Image *img, void *fileptr);

#endif
