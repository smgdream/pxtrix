// SPDX-License-Identifier: MIT
/* Copyright (c) 2024 Smgdream */

#ifndef LUT_HSV_H
#define LUT_HSV_H

#include "pixel.h"

static inline Px_rgba lut_hsv(float val)
{
	Px_hsva px = {val * 360.0f, 1.0f, 1.0f, 1.0f};
	return hsva2rgba(px);
}

#endif