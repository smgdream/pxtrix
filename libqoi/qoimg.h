/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#ifndef QOIMG_H
#define QOIMG_H

#include <stddef.h>
#include <stdint.h>

typedef struct image Image;


//enum qoi_channels_set { RGB = 3, RGBA = 4 };
//enum qoi_colorspace_set { sRGB_LA, ALL_LINEAR };

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

/* New a Qoimg object. Returns the pointer to it, or NULL on error.  */
Qoimg *qoi_new(uint32_t wid, uint32_t hgt, uint8_t channels, uint8_t colorspace);
/* Free a Qoimg object */
int qoi_free(Qoimg *qoi);

/* Read a QOI image file and create a Qoimg for it. 
 * Returns the pointer to Qoimg, or NULL if any error occurs.  */
Qoimg *qoi_read(const char *filename);
/* Write qoi to a QOI image file. Returns 0, or non-zero on error.  */
int qoi_write(const Qoimg *qoi, const char *filename);

/* Convert a Qoimg object to a Image object. Return img, or NULL on error.  */
Image *qoi2img(const Qoimg *qoi, Image *img);
/* Convert a Image object to a Image object. Return qoi, or NULL on error.  */
Qoimg *img2qoi(const Image *img, Qoimg *qoi);

/* Print the information of qoi to a file stream. Return non-zero on error.  */
int qoi_info(const Qoimg *qoi, void *fileptr);
/* As a argument. Be used to create a empty Qoimg.  */
#define QOI_EMPTY 0, 0, 4, 0

/* Other qoi operating function */

/* Decode the coded data in iobuf, save the decoded pixels to dest.
 * cnt is the number of pixels.  */
int qoi_decode(Qoi_px_def *dest, const Byte *iobuf, uint64_t cnt);
/* Encode the pixel from src, save the coded bytes to iosbuf.
 * cnt is the number of pixels.  */
size_t qoi_encode(Byte *iobuf, const Qoi_px_def *src, uint64_t cnt);
/* Reconfigure a Qoimg object.  */
Qoimg *qoi_set(Qoimg *qoi, uint32_t wid, uint32_t hgt, uint8_t channels, uint8_t colorspace);

#endif
