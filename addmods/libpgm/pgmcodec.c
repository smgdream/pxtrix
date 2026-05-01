// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#include <stddef.h>
#include <stdint.h>
#include "pgm.h"

union union16 {
	Byte_t pair[2];
	uint16_t val;
};

int pgm_decode(Pgm_px_def *pxbuf, const Byte_t *iobuf, size_t cnt, uint16_t maxval)
{
	union union16 tmp;

	/* uint16_t maxval > 65535 is impossible */
	if (pxbuf == NULL || iobuf == NULL || maxval == 0)
		return -1;

	if (maxval < 256) //px_siz == 1
		for (size_t s = 0, d = 0; cnt > 0; s += 1 /* px_siz */, ++d, --cnt)
			pxbuf[d] = *(uint8_t *)(iobuf + s) * (uint32_t) 65535 / maxval;
	else // px_siz == 2
		for (size_t s = 0, d = 0; cnt > 0; s += 2 /* px_siz */, ++d, --cnt) {
			tmp.pair[1] = iobuf[s + 0];
			tmp.pair[0] = iobuf[s + 1];
			pxbuf[d] = tmp.val * (uint32_t) 65535 / maxval;
		}

	return 0;
}

int pgm_encode(Byte_t *iobuf, const Pgm_px_def *pxbuf, size_t cnt, uint16_t maxval)
{
	union union16 tmp;

	/* uint16_t maxval > 65535 is impossible */
	if (pxbuf == NULL || iobuf == NULL || maxval == 0)
		return -1;
	
	if (maxval < 256) // px_siz == 1
		for (size_t s = 0, d = 0; cnt > 0; ++s, d += 1 /* px_siz */, --cnt)
			iobuf[d] = (uint8_t) (pxbuf[s] * (uint32_t) maxval / 65535);
	else // px_siz == 2
		for (size_t s = 0, d = 0; cnt > 0; ++s, d += 2 /* px_siz */, --cnt) {
			tmp.val = (uint16_t) (pxbuf[s] * (uint32_t) maxval / 65535);
			*(iobuf + d + 0) = tmp.pair[1];
			*(iobuf + d + 1) = tmp.pair[0];
		}
	
	return 0;
}