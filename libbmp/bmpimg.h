/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream
 *
 * Coordinate of Bmpimg
 *
 * y ^ 
 *   |
 *   |  bmp px
 *   |
 *   +----------> 
 * 0             x
 */

#ifndef BMPIMG_H
#define BMPIMG_H

#include <stddef.h>
#include <stdint.h>

#define BMP_TYPE_BM (0x4d42)


/* Portable version of bitmap file header */
typedef struct bmpfile_header_tag {
	uint16_t bf_type;		// The file type; must be 0x4d42 ("BM")
	/* a hole of 2 byte */
	uint32_t bf_size;		// Sizeof bitmap file, in byte.
	uint16_t bf_reserved1;	// Must be 0.
	uint16_t bf_reserved2;	// Must be 0.
	uint32_t bf_offbyte;	// The offset in byte, from fileheader to the bitmap bits.
} Bmpfile_header;

/* Portable version of bitmap info header */
typedef struct bmpinfo_header_tag {
	uint32_t bi_size;			// Sizeof this structure.
	int32_t  bi_width;			// Width of this bmp image, in pixels.
	int32_t  bi_height;			// Height of this bmp image, in pixels.
	uint16_t bi_planes;			// The number of the target device, must be 1.
	uint16_t bi_bitcount;		// Bits per pixel.
	uint32_t bi_compression;	// Number of compress level, 0: uncomperss, 1 or 2: compress.
	uint32_t bi_sizeimage;		// Sizeof the image(%4==0), This can be set to 0 for uncompress.
	int32_t  bi_x_px_permeter;	// horizontal resolution, in pixels per meter.
	int32_t  bi_y_px_permeter;	// vertical resolution, in pixels per meter.
	uint32_t bi_clrused;		// Number of colors used, if 0 mean clrused == 2^bpp.
	uint32_t bi_clrimportant;	// Number of important colors, if 0 mean all colors are important.
} Bmpinfo_header;

typedef struct bmp_px_rgb24 {
	uint8_t b; // blue
	uint8_t g; // green
	uint8_t r; // red
} Bmp_px_rgb24, Bmp_px_def;

typedef struct bitmap_image {
	Bmpfile_header bfh;
	Bmpinfo_header bih;
	Bmp_px_def *buf; // pixel buffer
} Bmpimg;


/* Bitmap image process function */

/* Basic */

/* New a Bmpimg object. Returns pointer to Bmpimg, or NULL on error.  */
Bmpimg *bmp_new(int32_t wid, int32_t hgt, uint16_t bpp);
/* Free a Bmpimg object. Return non-zero on error.  */
int bmp_free(Bmpimg *bmp);

/* Read a bitmap image file and create a new Image object for it.
 * Return the pointer to this Image object, or NULL if any error occurs.  */
Bmpimg *bmp_read(const char *filename);
/* Write the bmp to a bitmap image file. 
 * Return 0, or non-zero if any error occurs.  */
int bmp_write(const Bmpimg *bmp, const char *filename);


/* Print the information in bmp, to a file stream. 
 * Return non-zero on error. */
int bmp_info(const Bmpimg *bmp, void *fileptr);
/* Be used as a argument to create a empty Bmpimg. */
/* Test a file is a vaild bmp file or not.
 * Returns true (non-zero) if is or false (i.e. zero) if isn't.  */
int bmp_valid(const char *filename);
#define BMP_EMPTY 0, 0, 24

/* Extension */

/* Reconfigure a Bmpfile_header.  */
int bmp_set_fileheader(Bmpfile_header *bfhp, int32_t wid, int32_t hgt);
/* Reconfigure a Bmpinfo_header.  */
int bmp_set_infoheader(Bmpinfo_header *bihp, int32_t wid, int32_t hgt, uint16_t bpp);
/* Reconfigure the Bmpimg object. */
Bmpimg *bmp_set(Bmpimg *bmp, int32_t wid, int32_t hgt, uint16_t bpp);

/* Calculate the real size of a line of pixels in bmp file.  */
static inline size_t bmp_sizeof_line(int32_t wid)
{
	return (3*wid % 4 == 0) ? 3*wid : (3*wid / 4 + 1) * 4;
}

/* Returns the pointer to a pixel at (x, y) from bmp, 
 * or NULL if something wrong.  */
static inline Bmp_px_def *
bmp_px(const Bmpimg *bmp, size_t x, size_t y)
{
	if (bmp == NULL || (int32_t)x >= bmp->bih.bi_width || (int32_t)y >= bmp->bih.bi_height)
		return NULL;
	return &bmp->buf[x + y * bmp->bih.bi_width];
}


#endif
