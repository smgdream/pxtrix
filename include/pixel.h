// SPDX-License-Identifier: MIT
/* Copyright (c) 2024 Smgdream */

#ifndef PIXEL_H
#define PIXEL_H

#include <stdint.h>
#include <math.h>


typedef struct px_rgba_float32 {
	float r; // red		[0, 1]
	float g; // green	[0, 1]
	float b; // blue	[0, 1]
	float a; // alpha	[0, 1]
} Px_rgba_float32, Px_rgba_f32, Px_rgba, Px_def;

typedef struct px_hsva_float {
	float h; // Hue			[0, 360]
	float s; // Saturation	[0, 1]
	float v; // Value		[0, 1]
	float a; // Alpha		[0, 1]
} Px_hsva_float, Px_hsva_f32, Px_hsva;
/* CAUTION: The value of hue may equal to 360 */

typedef struct px_rgba_32bit {
	uint8_t r; // red	[0, 255]
	uint8_t g; // green	[0, 255]
	uint8_t b; // blue	[0, 255]
	uint8_t a; // alpha	[0, 255]
} Px_rgba_32bit, Px_rgba32_t, Px_rgba_8888;

/* color mapping devation rate: 56.2182% */
/* Mapping RGBA pixel to HSVA pixel */
Px_hsva rgba2hsva(Px_rgba rgba);
/* Mapping HSVA pixel to RGBA pixel */
Px_rgba hsva2rgba(Px_hsva hsva);

/* Alpha blending */
Px_rgba alpha_blend(Px_rgba dest, Px_rgba src);

/* Create a value of RGBA pixel by 4 channel value.
 * The domain of each channel is [0, 255]  */
static inline Px_rgba rgba_4val(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	Px_rgba px = {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
	return px;
}

/* Create a value of RGBA pixel by an eight numbers hexadecimat
 * sequence 0xRRGGBBAA. e.g. 0x39C5BBF  */
static inline Px_rgba rgba_hex(uint32_t hex_val)
{
	return rgba_4val(hex_val >> 24 & 0xff,
					 hex_val >> 16 & 0xff,
					 hex_val >> 8  & 0xff,
					 hex_val >> 0  & 0xff
	);
}

/* Pixel equal. Returns 1 if equal 0 or not  */
static inline int px_eq(Px_def px1, Px_def px2)
{
	return px1.r == px2.r &&
		   px1.g == px2.g && 
		   px1.b == px2.b && 
		   px1.a == px2.a;
}


#define _PX_AEQ_ERR (1.0f / 262144.0f)
/* Pixel almost equal. Returns 1 if almost equal 0 or not.
 * The current implementation ensures that adjacent color in RGBA hypercube of
 * 64 bpp is not almost equal  */
static inline int px_aeq(Px_def px1, Px_def px2)
{
	return fabsf(px1.r - px2.r) < _PX_AEQ_ERR &&
		   fabsf(px1.g - px2.g) < _PX_AEQ_ERR &&
		   fabsf(px1.b - px2.b) < _PX_AEQ_ERR &&
		   fabsf(px1.a - px2.a) < _PX_AEQ_ERR;
}
#undef _PX_AEQ_ERR

#endif
