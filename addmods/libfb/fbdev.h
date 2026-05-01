// SPDX-License-Identifier: MIT
/* Copyright (c) 2025 Smgdream */

/* 
 * Coordinate of Fbdev
 * 
 * 0             x
 *   +---------->
 *   |
 *   |    fb
 *   |
 * y v
 */

#ifndef FBDEV_H
#define FBDEV_H

#include <stddef.h>
#include <stdint.h>

enum fbdev_type_set { FB_FILE = 0, FB_DEVICE = 1 };

struct fb_base_info {
	uint32_t xres; // x resolution
	uint32_t yres; // y resolution
};

typedef struct fb_px_def {
	uint8_t b; // blue
	uint8_t g; // green
	uint8_t r; // red
	uint8_t a; // alpha
} Fb_px_def;

typedef struct fbdev {
	uint32_t xres;		// x resolution
	uint32_t yres;		// y resolution
	uint8_t dev_type;	// 0 = FB-file, 1 = FB-device
	Fb_px_def *buf;
} Fbdev;


/* Create a Fbdev object. Returns a pointer to this, or NULL if any error occurs.
 * The defaul color of image is #00000000.  */
Fbdev *fb_new(uint32_t xres, uint32_t yres, uint8_t dev_type);
/* Free a Fbdev object.
 * Returns 0, or non-zero on error.  */
int fb_free(Fbdev *fbd);

/* Read a framebuffer device/file and create a Fbdev object for it.
 * Retruns a pointer to the Fbdev object, or NULL if any error occurs.  */
Fbdev *fb_read(const char *filename);
/* Write the Fbdev object object to a framebuffer device/file.
 * Returns 0, non-zero on error.  */
int fb_write(const Fbdev *fbd, const char *filename);

/* Print the info of specifical Fbdev object to a stream (fileptr).
 * Returns 0, or non-zero on error.  */
int fb_info(const Fbdev *fbd, void *fileptr);
/* Test a Fbdev device/file is valid or not.
 * Returns true (non-zero) if valid, or false (i.e. 0) if invalid.  */
int fb_valid(const char *filename);

/* Used to new or set a empty Fbdev object.  */
#define FB_EMPTY(dev_type) 0, 0, dev_type

/* Reconfigure a Fbdev object. Returns fbd, or NULL if any error occurs. 
 * If the width or the height of the image has change. The content of image
 * is implementation-defined. */
Fbdev *fb_set(Fbdev *fbd, uint32_t xres, uint32_t yres, uint8_t dev_type);

/* Write the Fbdev object to a file stream. Returns non-zero on error.
 * The postiton of the stream must be at the begin of the file. After writeing
 * file finish this function will set the stream position to the begin of the 
 * file.  */
int fb_write_stream(const Fbdev *fbd, void *filestream);

#ifdef __linux__
/* Get framebuffer display information (only for Linux). Returns non-zero
 * on error. Assign x resolution to *xres, assign the y resolution to *yres.  */
int fb_display_info(const char *device_path, uint32_t *xres, uint32_t *yres);
#endif

/* Returns pointer to a framebuffer pixel which at (x, y) in fbd. */
static inline Fb_px_def *fb_px(const Fbdev *fbd, uint32_t x, uint32_t y)
{
	if (fbd == NULL || x >= fbd->xres || y >= fbd->yres)
		return NULL;
	return &fbd->buf[x + y * fbd->xres];
}

#endif