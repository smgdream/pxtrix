// SPDX-License-Identifier: MIT
/* Copyright (c) 2025 Smgdream */

/* 
 * Coordinate of Chart
 * 
 * 0             x
 *   +---------->
 *   |
 *   |   ca
 *   |
 * y v
 */

/* Chart mean character art. a.k.a. ASCII Art */

#ifndef CHART_H
#define CHART_H

#include <stddef.h>
#include <stdint.h>

#define _CA_TERM_D4_WID 80


typedef uint8_t Ca_px_def; // [0, 255]

typedef struct chart {
	uint16_t width;	// width
	uint16_t height;// height
	uint16_t cols;	// character per line
	Ca_px_def *buf;	// pxiel buffer
} Chart;

/* new a Chart object. Returns pointer to it, or NULL if any error occurs.
 * cols specified the number of characters per line. The defaul color of image
 * is black. */
Chart *ca_new(uint16_t wid, uint16_t hgt, uint16_t cols);
/* Free the specifical Chart object.
 * Returns 0, or non-zero on error.  */
int ca_free(Chart *ca);

/* Write a Chart object to a stream (fileptr). Returns 0, or non-zero on error.
 * wid_rate used to control the scale rate of width. (normal: 1, domain: [1, 2]).  */
int ca_write(const Chart *ca, double wid_rate, void *fileptr);

/* Reconfigure a Chart object Returns ca, or NULL if any error occurs.
 * If the width or the height of the image has change. The content of image
 * is implementation-defined. */
Chart *ca_set(Chart *ca, uint16_t wid, uint16_t hgt, uint16_t cols);
/* Print the information of ca to a stream.
 * Returns 0, or non-zero on error.  */
int ca_info(const Chart *ca, void *fileptr);
/* No need for ca_vaild */

/* Get the specifical pixel at (x, y).
 * Returns pointer to the pixel (Ca_px_def), or NULL if any error occurs.  */
static inline Ca_px_def *ca_px(const Chart *ca, uint16_t x, uint16_t y)
{
	if (ca == NULL || ca->buf == NULL)
		return NULL;
	return &ca->buf[x + y * ca->width];
}

/* CA_EMPTY used to create or set an empty Chart object */
#define CA_EMPTY 0, 0, _CA_TERM_D4_WID

#endif