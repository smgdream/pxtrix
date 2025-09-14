/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

#ifndef PIC2IMG_H
#define PIC2IMG_H

#include "image.h"
#include "bmpimg.h"
#include "qoimg.h"
#include "pgm.h"
#include "fbdev.h"
#include "chart.h"

/* Convert a Bmpimg object to a Image object. Return img, or NULL on error.
 * When size of pixel buffer of img is unsuitable it will reallocate the
 * pixel buffer automatically.  */
Image *bmp2img(const Bmpimg *bmp, Image *img);
/* Convert a Image object to a Bmpimg object. Return bmp, or NULL on error.
 * When size of pixel buffer of img is unsuitable it will reallocate the
 * pixel buffer automatically.  */
Bmpimg *img2bmp(const Image *img, Bmpimg *bmp);

/* Convert a Qoimg object to a Image object. Return img, or NULL on error. 
 * When size of pixel buffer of img is unsuitable it will reallocate the
 * pixel buffer automatically.  */
Image *qoi2img(const Qoimg *qoi, Image *img);
/* Convert a Image object to a Image object. Return qoi, or NULL on error.
 * When size of pixel buffer of qoi is unsuitable it will reallocate the
 * pixel buffer automatically.  */
Qoimg *img2qoi(const Image *img, Qoimg *qoi);

/* Convert the Pgm object to Image object.
 * If the size of pixels buffer of Image object is unsuitable
 * it will reallocate the memory area of pixels buffer.
 * Returns img, or NULL if any error occurs.  */
Image *pgm2img(const Pgm *p5, Image *img);
/* Convert the Image object to Pgm object.
 * If the size of pixels buffer of Pgm object is unsuitable
 * it will reallocate the memory area of pixels buffer.
 * The parameter gray accept a callback function which used to compute the
 * gray value. If using NULL as actual argument of gray it will use a built-in
 * function to compute gray value.
 * Returns p5, or NULL if any error occurs.  */
Pgm *img2pgm(const Image *img, Pgm *p5, uint8_t (*gray)(const Px_def *px));

/* Convert the Fbdev object to Image object.
 * If the size of pixels buffer of img is unsuitable.
 * It will reallocate the pixels buffer of img. 
 * Returns img, or NULL if any error occurs.   */
Image *fb2img(const Fbdev *fbd, Image *img);
/* Convert the Image object to Fbdev object. 
 * If the size of pixels buffer of fbd is unsuitable.
 * It will reallocate the pixels buffer of fbd.
 * Returns fbd, or NULL if any error occurs.  */
Fbdev *img2fb(const Image *img, Fbdev *fbd);

/* Convert the Image object to Chart object.
 * If the size of pixels buffer of ca is unsuitable.
 * It will reallocate the pixels buffer of ca.
 * Returns ca, or NULL if any error occurs.  */
Chart *img2ca(const Image *img, Chart *ca);

#endif