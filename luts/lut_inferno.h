// SPDX-License-Identifier: MIT
/* Copyright (c) 2024 Smgdream */

#ifndef LUT_INFERNO_H
#define LUT_INFERNO_H

#include "pixel.h"

static inline Px_rgba lut_inferno(float val)
{
	Px_rgba px = {0, 0, 0, 1};
	/* Thanks for Andyccr's help */
	const double fact_r[8] = { -0.001306,    0.109015,   11.373893,  -39.818824,
							   71.217502,  -62.279155,   18.542641,    1.830112};
	const double fact_g[8] = { -0.014869,    1.317262,  -14.065609,   73.312300, 
							 -186.071044,  251.271110, -169.571819,   44.825766};
	const double fact_b[8] = {  0.015321,    0.869181,   28.192310, -203.729879, 
							  602.981507, -914.973630,  691.507763, -204.230640};

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
