/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream
 * 
 * Coordinate of Chart
 * 
 * 0             x
 *   +---------->
 *   |
 *   |   ca
 *   |
 * y v
 */

#ifndef CHART_H
#define CHART_H

#include <stddef.h>
#include <stdint.h>

/* Chart mean character art. a.k.a. ASCII Art */

typedef struct ca_px_def {
	uint8_t r; // red
	uint8_t g; // green
	uint8_t b; // blue
} Ca_px_def;

typedef struct chart {
	uint16_t width;	// width
	uint16_t height;// height
	uint16_t cols;	// units per line (a unit = 2 characters)
	Ca_px_def *buf;	// pxiel buffer
} Chart;

/* new a chart object.
 * Returns pointer to it, or NULL if any error occurs.  */
Chart *ca_new(uint16_t wid, uint16_t hgt, uint16_t cols);
/* Free the specifical Chart object.
 * Returns 0, or non-zero on error.  */
int ca_free(Chart *ca);
/* Write a Chart object to a stream (fileptr).
 * wid_rate used to control the scale rate of width. (normal: 1) 
 * Returns 0, or non-zero on error.  */
int ca_write(const Chart *ca, void *fileptr, double wid_rate);
/* Reconfigure a Chart object
 * Returns ca, or NULL if any error occurs.  */
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

#endif