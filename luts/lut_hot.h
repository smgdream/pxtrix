// SPDX-License-Identifier: MIT
/* Copyright (c) 2024 Smgdream */

#ifndef LUT_HOT_H
#define LUT_HOT_H

#include "pixel.h"

#define _LUT_HOT_R_X1 0.0f
#define _LUT_HOT_R_X2 (105.0f/255.0f)
#define _LUT_HOT_R_Y1 0.0f
#define _LUT_HOT_R_Y2 1.0f
#define _LUT_HOT_G_X1 (101.0f/255.0f)
#define _LUT_HOT_G_X2 (210.0f/255.0f)
#define _LUT_HOT_G_Y1 0.0f
#define _LUT_HOT_G_Y2 1.0f
#define _LUT_HOT_B_X1 (201.0f/255.0f)
#define _LUT_HOT_B_X2 1.0f
#define _LUT_HOT_B_Y1 0.0f
#define _LUT_HOT_B_Y2 1.0f

#define _LUT_HOT_R_K ((_LUT_HOT_R_Y1 - _LUT_HOT_R_Y2) / (_LUT_HOT_R_X1 - _LUT_HOT_R_X2))
#define _LUT_HOT_G_K ((_LUT_HOT_G_Y1 - _LUT_HOT_G_Y2) / (_LUT_HOT_G_X1 - _LUT_HOT_G_X2))
#define _LUT_HOT_B_K ((_LUT_HOT_B_Y1 - _LUT_HOT_B_Y2) / (_LUT_HOT_B_X1 - _LUT_HOT_B_X2))

static inline Px_rgba lut_hot(float val)
{
	Px_rgba px = {0, 0, 0, 1};

	px.r = (val <= _LUT_HOT_R_X2) ? _LUT_HOT_R_K * val : 1;
	px.g = (val >= _LUT_HOT_G_X1 && val <= _LUT_HOT_G_X2) ?
			_LUT_HOT_G_K * (val - _LUT_HOT_G_X1) :
			(val < _LUT_HOT_G_X1) ? 0 : 1; // eq to val < 101.0f/255.0f
	px.b = (val < _LUT_HOT_B_X1) ? 0 : _LUT_HOT_B_K * (val - _LUT_HOT_B_X1);
	
	return px;
}

#undef _LUT_HOT_B_K
#undef _LUT_HOT_G_K
#undef _LUT_HOT_R_K

#undef _LUT_HOT_R_X1
#undef _LUT_HOT_R_X2
#undef _LUT_HOT_R_Y1
#undef _LUT_HOT_R_Y2
#undef _LUT_HOT_G_X1
#undef _LUT_HOT_G_X2
#undef _LUT_HOT_G_Y1
#undef _LUT_HOT_G_Y2
#undef _LUT_HOT_B_X1
#undef _LUT_HOT_B_X2
#undef _LUT_HOT_B_Y1
#undef _LUT_HOT_B_Y2

#endif
