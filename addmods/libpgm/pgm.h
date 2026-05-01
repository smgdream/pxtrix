// SPDX-License-Identifier: MIT
/* Copyright (c) 2025 Smgdream */

/* 
 * Coordinate of Pgm
 * 
 * 0             x
 *   +---------->
 *   |
 *   |   p5
 *   |
 * y v
 */

#ifndef PGM_H
#define PGM_H

#include <stdint.h>

typedef uint8_t Byte_t;

/* This enum can use as the argument of maxval of pgm_new or pgm_set */
enum pgm_bit_depth_set {PGM_GRAY_4BIT  = 16		- 1, 
						PGM_GRAY_8BIT  = 256	- 1, 
						PGM_GRAY_10BIT = 1024	- 1, 
						PGM_GRAY_12BIT = 4096	- 1,
						PGM_GRAY_16BIT = 65536	- 1
};

typedef uint16_t Pgm_px_def; // [0, 65535]
typedef struct pgm {
	uint16_t width;  // width
	uint16_t height; // height
	uint16_t maxval; // max value of pixel [1, 65535]
	Pgm_px_def *buf; // pixels buffer
} Pgm;


/* New a Pgm object. Returns a pointer to it, or NULL if any error occurs. 
 * The defaul color of image is black.  */
Pgm *pgm_new(uint16_t wid, uint16_t hgt, uint16_t maxval);
/* Free a Pgm object which is p5 pointer to.
 * Returns 0, or non-zero on error.  */
int pgm_free(Pgm *p5);

/* Read a PGM image file (filename) and create a Pgm object for it.
 * Returns pointer to this Pgm object, or NULL if any error occurs.  */
Pgm *pgm_read(const char *filename);
/* Write a Pgm object to a PGM image file
 * Returns 0, or non-zero on error.  */
int pgm_write(const Pgm *p5, const char *name);

/* Print the information of specified Pgm object to the file stream (fileptr)
 * Returns 0, or non-zero on error. */
int pgm_info(const Pgm *p5, void *fileptr);
/* Test a file is a vaild PGM file or not.
 * Returns true (non-zero) if is or false (i.e. zero) if isn't.  */
int pgm_valid(const char *filename);
/* Used as arguments to new or set a empty Pgm object.  */
#define PGM_EMPTY 0, 0, PGM_GRAY_8BIT

/* Extension */

/* Reconfigure a specified Pgm object Returns p5, or NULL if any error occurs.
 * If the width or the height of the image has change. The content of image 
 * is implementation-defined.  */
Pgm *pgm_set(Pgm *p5, uint16_t wid, uint16_t hgt, uint16_t maxval);

/* Decode a pgm image. Returns Non-zero on error. cnt is number of pixel */
int pgm_decode(Pgm_px_def *pxbuf, const Byte_t *iobuf, size_t cnt, uint16_t maxval);
/* Encode a pgm image. Returns Non-zero on error. cnt is number of pixel */
int pgm_encode(Byte_t *iobuf, const Pgm_px_def *pxbuf, size_t cnt, uint16_t maxval);


#endif