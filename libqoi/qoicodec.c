/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "qoimg.h"
#include "qoicodec.h"

int qoi_decode(Qoi_px_def *dest, const Byte *iobuf, uint64_t cnt)
{
	Byte tmp = 0;
	Qoi_px_def px_tmp = { 0, 0, 0, 0 };
	Qoi_px_def palette[64];

	if (dest == NULL || iobuf == NULL || cnt == 0)
		return 1;

	/* Initlized */
	qoi_dec_init();
	for (int i = 0; i < 64; ++i)
		palette[i] = px_tmp;

	/* Decode loop */
	while (cnt != 0) {
		tmp = *iobuf++;

		if (is_rgb(tmp)) {
			dest->r = *iobuf++;
			dest->g = *iobuf++;
			dest->b = *iobuf++;
			dest->a = prev_px(dest)->a;
			palette[qhash(dest)] = *dest;
			dest++;
			cnt--;
		} else if (is_rgba(tmp)) {
			dest->r = *iobuf++;
			dest->g = *iobuf++;
			dest->b = *iobuf++;
			dest->a = *iobuf++;
			palette[qhash(dest)] = *dest;
			dest++;
			cnt--;
		} else if (is_run(tmp)) {
			px_tmp = *prev_px(dest);
			for (int i = (tmp & ~MASK_2BIT_TAG) + 1; i > 0; i--, dest++, cnt--)
				*dest = px_tmp;
			palette[qhash(dest - 1)] = *(dest - 1);
		} else if (is_index(tmp)) {
			*dest = palette[tmp & ~MASK_2BIT_TAG];
			dest++;
			cnt--;
		} else if (is_diff(tmp)) {
			dest->r = prev_px(dest)->r + ((int)((tmp & 0x30) >> 4) - 2);
			dest->g = prev_px(dest)->g + ((int)((tmp & 0x0C) >> 2) - 2);
			dest->b = prev_px(dest)->b + ((int)((tmp & 0x03) >> 0) - 2);
			dest->a = prev_px(dest)->a;
			palette[qhash(dest)] = *dest;
			dest++;
			cnt--;
		} else if (is_luma(tmp)) {
			int dg = (int)(tmp & 0x3F) - 32;
			dest->g = prev_px(dest)->g + dg;
			tmp = *iobuf++;
			dest->r = ((int)((tmp & 0xF0) >> 4) - 8) + dg + prev_px(dest)->r;
			dest->b = ((int)((tmp & 0x0F) >> 0) - 8) + dg + prev_px(dest)->b;
			dest->a = prev_px(dest)->a;
			palette[qhash(dest)] = *dest;
			dest++;
			cnt--;
		} else {
			fprintf(stderr, "error: \"%s\" unknow tag \'0x%02x\'", __func__, tmp);
			exit(7);
		}
	}
	return 0;
}


size_t qoi_encode(Byte *iobuf, const Qoi_px_def *src, uint64_t cnt)
{
	Byte tmp = 0;
	Qoi_px_def px_tmp = { 0, 0, 0, 0 };
	Qoi_px_def palette[64];
	size_t nbyte = 0;

	if (iobuf == NULL || src == NULL || cnt == 0)
		return 0;

	/* Initlize */
	qoi_enc_init();
	for (int i = 0; i < 64; ++i)
		palette[i] = px_tmp;

	/* Encode loop */
	while (cnt != 0) {
		if (can_use_run(src)) {
			uint8_t run = 0;
			for (; px_eq(src, prev_px(src)); run++, src++, cnt--)
				if (run >= 62 || cnt == 0)
					break;
			tmp = 0;
			tmp |= RUN_TAG;
			tmp |= run - 1;
			*iobuf++ = tmp;
			palette[qhash(src - 1)] = *(src - 1);
			nbyte++;
		} else if (can_use_index(src)) {	
			tmp = 0;
			tmp |= INDEX_TAG;
			tmp |= (uint8_t) qhash(src);
			*iobuf++ = tmp;
			src++;
			cnt--;
			nbyte++;
		} else if (can_use_diff(src)) {
			tmp = 0;
			tmp |= DIFF_TAG;
			tmp |= (uint8_t)(src->r - prev_px(src)->r + 2) << 4;
			tmp |= (uint8_t)(src->g - prev_px(src)->g + 2) << 2;
			tmp |= (uint8_t)(src->b - prev_px(src)->b + 2) << 0;
			*iobuf++ = tmp;
			palette[qhash(src)] = *src;
			src++;
			cnt--;
			nbyte++;
		} else if (can_use_luma(src)) {
			int dg = src->g - prev_px(src)->g;
			int dr_dg = src->r - prev_px(src)->r - dg;
			int db_dg = src->b - prev_px(src)->b - dg;
			
			tmp = 0;
			tmp |= LUMA_TAG;
			tmp |= (uint8_t) (dg + 32);
			*iobuf++ = tmp;
			tmp = 0;
			tmp |= (uint8_t) (dr_dg + 8) << 4;
			tmp |= (uint8_t) (db_dg + 8) << 0;
			*iobuf++ = tmp;
			palette[qhash(src)] = *src;
			src++;
			cnt--;
			nbyte += 2;
		} else if (can_use_rgb(src)) {
			*iobuf++ = RGB_TAG;
			*iobuf++ = src->r;
			*iobuf++ = src->g;
			*iobuf++ = src->b;
			palette[qhash(src)] = *src;
			src++;
			cnt--;
			nbyte += 4;
		} else { // RGBA
			*iobuf++ = RGBA_TAG;
			*iobuf++ = src->r;
			*iobuf++ = src->g;
			*iobuf++ = src->b;
			*iobuf++ = src->a;
			palette[qhash(src)] = *src;
			src++;
			cnt--;
			nbyte += 5;
		}
	}
	return nbyte;
}
