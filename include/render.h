/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include "image.h"

/* Convert an RGBA image to grayscale.
 * f is a pointer to a callback function which used to compute the gray value
 * of specified rgba pixel (pointer to Px_def: px) given by the gray function.
 * The vaild range of return value of f is [0, 255]. 
 * If the actual argument of f is NULL. It will using a built-in gray value
 * compute function to computing the gray value of the rgba pixel.  */
Image *gray(Image *img, uint8_t (*f)(const Px_def *px));
/* Image thersholding */
Image *bin(Image *img, uint8_t threshold);
/* Fill the canvas with specify color */
Image *fill(Image *img, const Px_def *clr);
/* draw a line parallel to x axis */
Image *line_row(Image *img, uint32_t y, uint32_t weight, const Px_def *clr);
/* draw a line parallel to y axis */
Image *line_col(Image *img, uint32_t x, uint32_t weight, const Px_def *clr);


#define ct_x(x, img) ((uint32_t) ( (x) * 100.0 + ((img)->width >> 1) ))
#define ct_y(y, img) ((uint32_t) ( (y) * 100.0 + ((img)->height >> 1) ) )
#define ct_x_edge(img) ((double) ( ((img)->width >> 1) / 100.0 ) )
#define ct_y_edge(img) ((double) ( ((img)->height >> 1) / 100.0 ) )

#endif
