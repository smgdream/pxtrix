// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#include "pixel.h"

#define COLOR_MIKU_HEX 0x39c5bbff
/* Forna pixiv id: 78937570 */
#define COLOR_FORNA_DRESS_BURGUNDY_4VAL 107, 23, 39, 255
#define COLOR_FORNA_4VAL COLOR_FORNA_DRESS_BURGUNDY_4VAL

int main(void)
{
	Px_rgba px = {0x39/255.0f, 0xc5/255.0f, 0xbb/255.0f, 0xff/255.0f};

	if (!px_eq(px, rgba_hex(COLOR_MIKU_HEX)))
		return __LINE__;
	Px_def px0 = hsva2rgba(rgba2hsva(rgba_4val(COLOR_FORNA_4VAL)));
	if (!px_aeq( px0, alpha_blend(rgba_4val(COLOR_FORNA_4VAL), rgba_hex(0xffffff00)) ))
		return __LINE__;

	return 0;
}