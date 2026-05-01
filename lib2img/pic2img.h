// SPDX-License-Identifier: MIT
/* Copyright (c) 2025 Smgdream */

#ifndef PIC2IMG_H
#define PIC2IMG_H

#include "image.h"
#include "bmpimg.h"
#include "qoimg.h"
#include "pgm.h"
#include "fbdev.h"
#include "chart.h"

#ifndef _PIC2IMG_DE4_WITHOUT_ALL_
	#define _BMP2IMG_
	#define _QOI2IMG_
	#define _PGM2IMG_
	#define _FB2IMG_
	#define _CA2IMG_
#endif

#ifdef _BMP2IMG_
/* Convert a Bmpimg object to a Image object. Return img, or NULL on error.
 * When size of pixel buffer of img is unsuitable it will reallocate the
 * pixel buffer automatically.  */
Image *bmp2img(const Bmpimg *bmp, Image *img);
/* Convert a Image object to a Bmpimg object. Return bmp, or NULL on error.
 * When size of pixel buffer of img is unsuitable it will reallocate the
 * pixel buffer automatically.  */
Bmpimg *img2bmp(const Image *img, Bmpimg *bmp);
#endif

#ifdef _QOI2IMG_
/* Convert a Qoimg object to a Image object. Return img, or NULL on error. 
 * When size of pixel buffer of img is unsuitable it will reallocate the
 * pixel buffer automatically.  */
Image *qoi2img(const Qoimg *qoi, Image *img);
/* Convert a Image object to a Image object. Return qoi, or NULL on error.
 * When size of pixel buffer of qoi is unsuitable it will reallocate the
 * pixel buffer automatically.  */
Qoimg *img2qoi(const Image *img, Qoimg *qoi);
#endif

#ifdef _PGM2IMG_
/* Convert the Pgm object to Image object. Returns img, or NULL if any error occurs.
 * If the size of pixels buffer of Image object is unsuitable. It will reallocate
 * the memory area of pixels buffer automatically.  */
Image *pgm2img(const Pgm *p5, Image *img);
/* Convert the Image object to Pgm object. Returns p5, or NULL if any error occurs.
 * The parameter gris accept a callback function which used to compute the gray
 * value. The callback function accept a pixel of type Px_def as the argument. The
 * vaild range of return value of gris is [0, 1]. If using NULL as actual argument
 * of gris. It will use a built-in function to compute the gray value.
 * If the size of pixels buffer of Pgm object is unsuitable. It will reallocate
 * the memory area of pixels buffer automatically.  */
Pgm *img2pgm(const Image *img, Pgm *p5, float (*gris)(Px_def px));
#endif

#ifdef _FB2IMG_
/* Convert the Fbdev object to Image object. Returns img, or NULL if any error occurs.
 * If the size of pixels buffer of img is unsuitable. It will reallocate the pixels
 * buffer of img automatically.  */
Image *fb2img(const Fbdev *fbd, Image *img);
/* Convert the Image object to Fbdev object. Returns fbd, or NULL if any error occurs.
 * If the size of pixels buffer of fbd is unsuitable. It will reallocate the pixels 
 * buffer of fbd  automatically. However, went the device type of fbd is FB_DEVICE it
 * will restrain the reallocate.  */
Fbdev *img2fb(const Image *img, Fbdev *fbd);
#endif

#ifdef _CA2IMG_
/* Convert the Image object to Chart object. Returns ca, or NULL if any error occurs.
 * If the size of pixels buffer of ca is unsuitable. It will reallocate the pixels
 * buffer of ca automatically.  */
Chart *img2ca(const Image *img, Chart *ca);
#endif

#endif