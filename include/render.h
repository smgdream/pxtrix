// SPDX-License-Identifier: MIT
/* Copyright (c) 2024 Smgdream */

#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include "image.h"
#include "tmath.h"

/* Convert an RGBA image to grayscale. Returns img.
 * gris is a pointer to a callback function which used to compute the gray value
 * of specified rgba pixel given by the gray function. The vaild range of return
 * value of gris is [0, 1]. If the actual argument of gris is NULL. It will
 * using a built-in gray value compute function to computing the gray value of
 * the rgba pixel.  */
Image *gray(Image *img, float (*gris)(Px_def px));

/* Image thresholding. domain of threshold: [0, 1], Returns img.
 * gris is a pointer to a callback function which used to compute the gray value
 * of specified rgba pixel given by the bin function. The vaild range of return
 * value of gris is [0, 1]. If the actual argument of gris is NULL. It will
 * using a built-in gray value compute function to computing the gray value of
 * the rgba pixel.  */
Image *bin(Image *img, float (*gris)(Px_def px), float threshold);

/* Fill the canvas with specify color. Returns img or NULL on error. clr used
 * to set the color.  */
Image *fill(Image *img, Px_def clr);

/* Draw a line parallel to x axis. Returns img or NULL on error.  */
Image *line_row(Image *img, uint32_t y, uint32_t weight, Px_def clr);

/* Draw a line parallel to y axis. Returns img or NULL on error.  */
Image *line_col(Image *img, uint32_t x, uint32_t weight, Px_def clr);

/* Draw a cubic bezier curve. Returns img or NULL on error.  */
Image *bezier_curve(Image *img, Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, Px_def clr);

/* Resize image. Returns img or NULL on error.  */
Image *scale(Image *img, uint32_t wid, uint32_t hgt);

/* Crop image from (x,y) to (min(x+wid,img->width), min(y+hgt,img->height)),
 * exclude (min(x+wid, img->width), min(y+hgt, img->height)). Returns img or
 * NULL on error.  */
Image *crop(Image *img, uint32_t x, uint32_t y, uint32_t wid, uint32_t hgt);

/* Overlays src onto dest at (x,y). Only the part of src inside dest is drawn.
 * Returns dest or NULL on error.  */
Image *overlay(Image *dest, const Image *src, int64_t x, int64_t y);

/* Invert image. Returns img or NULL on error.  */
Image *invert(Image *img);

/* Flip vertical. Returns img or NULL on error.  */
Image *flip_v(Image *img);

/* Flip horizontal. Returns img or NULL on error.  */
Image *flip_h(Image *img);

#endif
