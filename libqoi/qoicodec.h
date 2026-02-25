/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#ifndef QOICODEC_H
#define QOICODEC_H

#include <stdint.h>
#include "qoimg.h"

#define RGB_TAG		(0xFE)
#define RGBA_TAG	(0xFF)
#define INDEX_TAG	(0x00)
#define DIFF_TAG	(0x40)
#define LUMA_TAG	(0x80)
#define RUN_TAG		(0xC0)

#define MASK_2BIT_TAG (0xC0)
#define MASK_8BIT_TAG (0xFF)

#define is_rgb(abyte)	( ((abyte) & MASK_8BIT_TAG) == RGB_TAG )
#define is_rgba(abyte)	( ((abyte) & MASK_8BIT_TAG) == RGBA_TAG )
#define is_index(abyte)	( ((abyte) & MASK_2BIT_TAG) == INDEX_TAG )
#define is_diff(abyte)	( ((abyte) & MASK_2BIT_TAG) == DIFF_TAG )
#define is_luma(abyte)	( ((abyte) & MASK_2BIT_TAG) == LUMA_TAG )
#define is_run(abyte)	( ((abyte) & MASK_2BIT_TAG) == RUN_TAG )

static inline size_t qhash(const Qoi_px_def *pxptr)
{ return (pxptr->r * 3 + pxptr->g * 5 + pxptr->b * 7 + pxptr->a * 11) % 64; }

static inline int px_eq(const Qoi_px_def *pxptr0, const Qoi_px_def *pxptr1)
{ return *(uint32_t *)pxptr0 == *(uint32_t *)pxptr1; }

#define prev_px(px_ptr) ((cnt == cnt_raw) ? &px_beg : (px_ptr - 1))

#define qoi_dec_init() \
	uint64_t cnt_raw = cnt; \
	Qoi_px_def px_beg = { 0, 0, 0, 255 }

#define qoi_enc_init() \
	uint64_t cnt_raw = cnt; \
	const Qoi_px_def px_beg = { 0, 0, 0, 255 }

#define can_use_run(pxptr) \
	( px_eq((pxptr), prev_px(pxptr)) )

#define can_use_index(pxptr) \
	( px_eq((pxptr), &palette[qhash(pxptr)]) )

#define can_use_diff(pxptr) ( \
	   (pxptr)->r - prev_px(pxptr)->r >= -2 && (pxptr)->r - prev_px(pxptr)->r <= 1 \
	&& (pxptr)->g - prev_px(pxptr)->g >= -2 && (pxptr)->g - prev_px(pxptr)->g <= 1 \
	&& (pxptr)->b - prev_px(pxptr)->b >= -2 && (pxptr)->b - prev_px(pxptr)->b <= 1 \
	&& (pxptr)->a == prev_px(pxptr)->a \
)

#define can_use_luma(pxptr) ( \
	(pxptr)->g - prev_px(pxptr)->g >= -32 && (pxptr)->g - prev_px(pxptr)->g <= 31 \
	&& ((pxptr)->r - prev_px(pxptr)->r) - ((pxptr)->g - prev_px(pxptr)->g) >= -8 \
	&& ((pxptr)->r - prev_px(pxptr)->r) - ((pxptr)->g - prev_px(pxptr)->g) <= 7 \
	&& ((pxptr)->b - prev_px(pxptr)->b) - ((pxptr)->g - prev_px(pxptr)->g) >= -8 \
	&& ((pxptr)->b - prev_px(pxptr)->b) - ((pxptr)->g - prev_px(pxptr)->g) <= 7 \
	&& (pxptr)->a == prev_px(pxptr)->a \
)

#define can_use_rgb(pxptr) \
	((pxptr)->a == prev_px(pxptr)->a)

/* can_use_rgba is no need. */

#endif
