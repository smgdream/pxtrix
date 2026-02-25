# Portable Bitmap Image structure

```c
/* bmpimg.h */

/* Portable version of bitmap file header */
typedef struct bmpfile_header_tag {
	uint16_t bf_type;		// The file type; must be 0x4d42 ("BM")
	/* a hole of 2 byte */
	uint32_t bf_size;		// Sizeof bitmap file, in byte.
	uint16_t bf_reserved1;	// Must be 0.
	uint16_t bf_reserved2;	// Must be 0.
	uint32_t bf_offbyte;	// The offset in byte, from fileheader to the bitmap bits.
} Bmpfile_header;
```

<br>

```c
/* bmpimg.h */

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
```

<br>

```c
/* bmpimg.h */

/* 24bpp bitmap color structure */
typedef struct bmp_px_rgb24 {
	uint8_t b; // blue
	uint8_t g; // green
	uint8_t r; // red
} Bmp_px_rgb24, Bmp_px_def;
```
注：在BMP文件中每个像素扫描行要与`uint32_t`对齐(即一行像素的空间占用大小能被4byte整除)，行末的空间可用`0x00`填充，以实现对齐。

## Reference

- [Microsoft Bitmap Image Format](ms-bmp-format.md)