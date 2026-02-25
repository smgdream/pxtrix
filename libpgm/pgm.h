/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream
 * 
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
#include "pixel.h"


typedef uint8_t Pgm_px_def;
typedef struct pgm {
	uint16_t width;  // width of image
	uint16_t height; // height of image
	uint16_t maxval; // max value of a pixel of pgm image
	Pgm_px_def *buf; // pixels buffer
} Pgm;


/* New a Pgm object.
 * Returns a pointer to it, or NULL if any error occurs.  */
Pgm *pgm_new(uint16_t wid, uint16_t hgt);
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
#define PGM_EMPTY 0, 0

/* Extension */

/* Reconfigure a specified Pgm object
 * Returns p5, or NULL if any error occurs.  */
Pgm *pgm_set(Pgm *p5, uint16_t wid, uint16_t hgt);


#endif