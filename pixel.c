// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#include <stddef.h>
#include "pixel.h"
#include "tmath.h"

/* ref:
 * Color Gamut Transform Pairs - Alvy Ray Smith
 * OpenCV HSV2RGB_native */
Px_rgba hsva2rgba(Px_hsva hsva)
{
	float h = hsva.h / 60.0f; // let 0 <= h <= 6
	float s = hsva.s;
	float v = hsva.v;

	int l = (int) h;
	float m = h - l;

	float d = v * (1.0f - s);
	float e = v * (1.0f - m*s);
	float f = v * (1.0f - (1.0f-m)*s);
	
	float r = 0, g = 0, b = 0;
	switch (l) {
	/* if H == 360 then l == 6 */
	case 0: case 6:
		r = v, g = f, b = d;
		break;
	case 1:
		r = e, g = v, b = d;
		break;
	case 2:
		r = d, g = v, b = f;
		break;
	case 3:
		r = d, g = e, b = v;
		break;
	case 4:
		r = f, g = d, b = v;
		break;
	case 5:
		r = v, g = d, b = e;
		break;
	}

	Px_def rgba = {r, g, b, hsva.a};
	return rgba;
}

Px_hsva rgba2hsva(Px_rgba rgba)
{
	float r = rgba.r;
	float g = rgba.g;
	float b = rgba.b;

	float mini = min3(r, g, b);
	float maxi = max3(r, g, b);
	float diff = maxi - mini;

	float v = maxi;
	float s = (v != 0) ? diff / v : 0;
	float h = 0;

	if (diff == 0)
		h = 0.0f;
	else if (v == r)
		h =   0.0f + 60.0f * (g-b) / diff;
	else if (v == g)
		h = 120.0f + 60.0f * (b-r) / diff;
	else // v == b
		h = 240.0f + 60.0f * (r-g) / diff;
	
	if (h < 0)
		h += 360.0f;
	
	Px_hsva hsva = {h, s, v, rgba.a};
	return hsva;
}

/* ref:
 * https://zhuanlan.zhihu.com/p/1962657973915591283
 * https://zhuanlan.zhihu.com/p/344751308 
 * https://handwiki.org/wiki/Alpha_compositing */
Px_rgba alpha_blend(Px_rgba dest, Px_rgba src)
{
	Px_rgba out = {0, 0, 0, 0};

	out.r = (src.a * src.r) + (dest.a * dest.r) * (1-src.a);
	out.g = (src.a * src.g) + (dest.a * dest.g) * (1-src.a);
	out.b = (src.a * src.b) + (dest.a * dest.b) * (1-src.a);
	out.a =  src.a + dest.a * (1-src.a);

	return out;
}
