// SPDX-License-Identifier: MIT
/* Copyright (c) 2024 Smgdream */

#ifndef LUT_MAGMA_H
#define LUT_MAGMA_H

#include "pixel.h"

static inline Px_rgba lut_magma(float val)
{
	Px_rgba px = {0, 0, 0, 1};
	/* Thanks for Andyccr's help */
	const double fact_r[8] = {  0.011104,   -0.605548,   19.927978,  -91.843425,
							  228.039235, -303.178078,  200.423543,  -51.784650};
	const double fact_g[8] = { -0.029403,    2.149405,  -23.625938,  126.645763,
							 -338.764562,  478.067024, -336.316014,   92.867012};
	const double fact_b[8] = {  0.014551,    1.030507,   19.948636, -121.833636, 
							  308.313240, -418.460183,  298.051793,  -86.344184};

	double prod = 1;
	for (int i = 0; i < 8; ++i) {
		px.r += fact_r[i] * prod;
		px.g += fact_g[i] * prod;
		px.b += fact_b[i] * prod;
		prod *= val;
	}
	px.r = fabsf(px.r);
	px.g = fabsf(px.g);
	px.b = fabsf(px.b);
	
	return px;
}

#endif
