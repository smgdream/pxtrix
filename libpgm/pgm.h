/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

#ifndef PGM_H
#define PGM_H

#include <stdint.h>
#include "pixel.h"

/* The following preprocessing directive is used to avoid the redefinition
 * of Image type.
 * 
 * Usage:
 * In the source file within libpgm, it should be define __LIBPGM_INSIDE__ before
 * including pgm.h. e.g.
 * 
 * #define __LIBPGM_INSIDE__
 * // ...
 * #include "pgm.h"
 * 
 * If the source file within libpgmb needs to include image.h also define
 * __LIBPGM_USE_IMAGE__ before including pgm.h. e.g.
 * 
 * #define __LIBPGM_INSIDE__
 * #define __LIBPGM_USE_IMAGE__
 * // ...
 * #include "pgm.h"
 * #include "image.h"
 * 
 * For the user of pxtrix, do not define __LIBPGM_USE_IMAGE__ or
 * __LIBPGM_INSIDE__ in the source files that use libpgm and/or image e.g.
 * // main.c
 * #include "image.h"
 * #include "pgm.h"  */
#ifdef __LIBPGM_INSIDE__
	#if !defined(__IMAGE_TYPE__) && !defined(__LIBPGM_USE_IMAGE__)
		typedef struct image Image;
	#endif
	#ifdef __LIBPGM_USE_IMAGE__
		#include "image.h"
	#endif
#else
	#include "image.h"
#endif

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

/* Convert the Pgm object to Image object.
 * If the size of pixels buffer of Image object is unsuitable
 * it will reallocate the memory area of pixels buffer.
 * Returns img, or NULL if any error occurs.  */
Image *pgm2img(const Pgm *p5, Image *img);
/* Convert the Image object to Pgm object.
 * If the size of pixels buffer of Pgm object is unsuitable
 * it will reallocate the memory area of pixels buffer.
 * The parameter gray accept a callback function which used to compute the gray value.
 * If using NULL as actual argument of gray it will use a built-in function to compute gray value.
 * Returns p5, or NULL if any error occurs.  */
Pgm *img2pgm(const Image *img, Pgm *p5, uint8_t (*gray)(const Px_def *px));

/* Print the information of specified Pgm object to the file stream (fileptr)
 * Returns 0, or non-zero on error. */
int pgm_info(const Pgm *p5, void *fileptr);
/* Test a file is PGM file or not.
 * Returns true (non-zero) if is or false (i.e. zero) if isn't.  */
int pgm_test(const char *filename);
/* Used as arguments to new or set a empty Pgm object.  */
#define PGM_EMPTY 0, 0

/* Extension */

/* Reconfigure a specified Pgm object
 * Returns p5, or NULL if any error occurs.  */
Pgm *pgm_set(Pgm *p5, uint16_t wid, uint16_t hgt);


#endif