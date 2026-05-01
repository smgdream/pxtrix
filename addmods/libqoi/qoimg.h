// SPDX-License-Identifier: MIT
/* Copyright (c) 2024 Smgdream */

/* 
 * Coordinate of Qoimg
 * 
 * 0             x
 *   +---------->
 *   |
 *   |   qoi
 *   |
 * y v
 */

#ifndef QOIMG_H
#define QOIMG_H

//#include <stdio.h> //////
#include <stddef.h>
#include <stdint.h>

enum qoi_channels_set { QOI_CH_RGB = 3, QOI_CH_RGBA = 4 };
enum qoi_colorspace_set {QOI_GAMUT_SRGB_LA = 0, QOI_GAMUT_ALL_LINEAR = 1 };

typedef struct qoi_header {
	char magic[4];		// magic bytes "qoif"
	uint32_t width;		// image width in pixels (BE)
	uint32_t height;	// image height in pixels (BE)
	uint8_t channels;	// 3 = RGB, 4 = RGBA
	uint8_t colorspace;	// 0 = sRGB with linear alpha
						// 1 = all channels linear
} Qoi_header;

typedef struct qoi_px_rgb {
	uint8_t r; // red
	uint8_t g; // green
	uint8_t a; // blue
} Qoi_px_rgb;

typedef struct qoi_px_rgba {
	uint8_t r; // red
	uint8_t g; // green
	uint8_t b; // blue
	uint8_t a; // alpha
} Qoi_px_rgba, Qoi_px_def;

typedef struct qoimg {
	Qoi_header header;
	Qoi_px_def *buf;
} Qoimg;

typedef uint8_t Byte;

/* Basic qoi operating function */

/* New a Qoimg object. Returns the pointer to it, or NULL on error.
 * The defaul color of image is #00000000.  */
Qoimg *qoi_new(uint32_t wid, uint32_t hgt, uint8_t channels, uint8_t colorspace);
/* Free a Qoimg object. Return 0, or non-zero on error */
int qoi_free(Qoimg *qoi);

/* Read a QOI image file and create a Qoimg for it. 
 * Returns a pointer to Qoimg, or NULL if any error occurs.  */
Qoimg *qoi_read(const char *filename);
/* Write qoi to a QOI image file. Returns 0, or non-zero on error.  */
int qoi_write(const Qoimg *qoi, const char *filename);

/* Print the information of qoi to a file stream. Return non-zero on error.  */
int qoi_info(const Qoimg *qoi, void *fileptr);
/* Test a file is a vaild QOI file or not.
 * Returns true (non-zero) if is or false (i.e. zero) if isn't.  */
int qoi_valid(const char *filename);
/* As a argument. Be used to create a empty Qoimg.  */
#define QOI_EMPTY 0, 0, QOI_CH_RGBA, QOI_GAMUT_SRGB_LA

/* Other qoi operating function */

/* Decode the coded data in iobuf, save the decoded pixels to dest.
 * cnt is the number of pixels.  */
int qoi_decode(Qoi_px_def *dest, const Byte *iobuf, uint64_t cnt);
/* Encode the pixel from src, save the coded bytes to iosbuf.
 * cnt is the number of pixels.  */
size_t qoi_encode(Byte *iobuf, const Qoi_px_def *src, uint64_t cnt);
/* Reconfigure a Qoimg object. Returns qoi or NULL if any error occurs.
 * If the width or the height of the image has change. The content of image 
 * is implementation-defined.  */
Qoimg *qoi_set(Qoimg *qoi, uint32_t wid, uint32_t hgt, uint8_t channels, uint8_t colorspace);

/* Read the QOI image file stream and create a Qoimg object for it. Returns a
 * pointer to Qoimg, or NULL if any error occurs. The position of the file must
 * at the begin of a qoi image data segment. After reading stream finish this
 * function will set the file position to the begin of the file.  */
Qoimg *qoi_read_stream(void *filestream);
/* Test the stream is valid qoi stream or not. Returns true if valid or false if invalid.
 * The position of the file should at the begin of qoi image data segment. This
 * function will not change the file position of the stream  */
int qoi_valid_stream(void *stream);

static inline Qoi_px_def *qoi_px(const Qoimg *qoi, uint32_t x, uint32_t y)
{
	if (qoi == NULL || x >= qoi->header.width || y >= qoi->header.height)
		return NULL;
	return &qoi->buf[x + y * qoi->header.width];
}

#endif
