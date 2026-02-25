/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#ifndef PIXEL_H
#define PIXEL_H

#include <stdint.h>

typedef struct px_rgba_32bit {
	uint8_t r; // red	[0, 255]
	uint8_t g; // green	[0, 255]
	uint8_t b; // blue	[0, 255]
	uint8_t a; // alpha	[0, 255]
} Px_rgba_32bit, Px_rgba32_t, Px_rgba, Px_def;

typedef struct px_hsva_float {
	float h; // Hue			[0, 360]
	float s; // Saturation	[0, 1]
	float v; // Value		[0, 1]
	float a; // Alpha		[0, 1]
} Px_hsva_float, Px_hsva_f, Px_hsva;

/* Create a value of RGBA pixel by an eight numbers hexadecimat
 * sequence 0xRRGGBBAA. e.g. 0x39C5BBFF */
static inline Px_rgba rgba_hex(uint32_t hex_val)
{
	Px_rgba px = {	(hex_val) >> 24 & 0xff,
					(hex_val) >> 16 & 0xff,
					(hex_val) >> 8  & 0xff,
					(hex_val) & 0xff
				};
	return px;
}

/* color mapping devation rate: 56.2182% */
/* Mapping RGBA pixel to HSVA pixel */
Px_hsva rgba2hsva(Px_def rgba);
/* Mapping HSVA pixel to RGBA pixel */
Px_def hsva2rgba(Px_hsva hsva);

#endif
