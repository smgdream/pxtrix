// SPDX-License-Identifier: MIT
/* Copyright (c) 2025 Smgdream */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "chart.h"

static size_t _ca_iobuf_size(const Chart *ca);
static int _ca_px2aa(char *iobuf, const Chart *ca, double rate);

Chart *ca_new(uint16_t wid, uint16_t hgt, uint16_t cols)
{
	Chart *tmp = NULL;
	
	if ((tmp = malloc(sizeof(Chart))) == NULL)
		return NULL;
		
	tmp->buf = NULL;
	if (wid * hgt != 0 && (tmp->buf = calloc(wid * hgt, sizeof(Ca_px_def))) == NULL) {
		free(tmp);
		return NULL;
	}

	tmp->width = wid;
	tmp->height = hgt;
	tmp->cols = cols;
	
	return tmp;
}

int ca_free(Chart *ca)
{
	if (ca == NULL)
		return -1;
		
	if (ca->buf != NULL)
		free(ca->buf);
	free(ca);
	
	return 0;
}

/* The argument rate is scale rate of width (valid range [1, 2]). */
int ca_write(const Chart *ca, double rate, void *fptr)
{
	char *iobuf = NULL;
	FILE *fp = (FILE *) fptr;
	
	if (ca == NULL || fp == NULL || rate < 1 || rate > 2
	 || ca->height * ca->width == 0)
		return -1;

	if ((iobuf = calloc(1, _ca_iobuf_size(ca))) == NULL)
		return -1;
	_ca_px2aa(iobuf, ca, rate);
	fprintf(fp, "%s", iobuf);

	free(iobuf);
	return 0;
}

Chart *ca_set(Chart *ca, uint16_t wid, uint16_t hgt, uint16_t cols)
{
	if (ca == NULL)
		return NULL;

	if (wid * hgt == 0 && ca->buf != NULL) {
		free(ca->buf);
		ca->buf = NULL;
	}

	/* reallocation pixel buffer */
	if (wid * hgt != ca->width * ca->height && wid * hgt != 0) {
		Ca_px_def *new_buf = NULL;

		if ((new_buf = calloc(wid * hgt, sizeof(Ca_px_def))) == NULL)
			return NULL;
		if (ca->buf != NULL)
			free(ca->buf);
		ca->buf = new_buf;
	}

	ca->width = wid;
	ca->height = hgt;
	ca->cols = cols;

	return ca;
}

int ca_info(const Chart *ca, void *fptr)
{
	FILE *fp = (FILE *) fptr;
	
	if (ca == NULL || fp == NULL)
		return -1;

	fprintf(fp, "%-12s%"PRIu16"\n",	"width",   ca->width);
	fprintf(fp, "%-12s%"PRIu16"\n", "height",  ca->height);
	fprintf(fp, "%-12s%"PRIu16"\n", "columns", ca->cols);
	return 0;
}

#define _CA_LF_SIZE (sizeof((char)'\n'))
#define _CA_NUL_SIZE (sizeof((char)'\0'))

static size_t _ca_iobuf_size(const Chart *ca)
{
	if (ca == NULL)
		return 0;
	size_t rows = (ca->cols * ca->height) / (ca->width * 2);
	return rows * (ca->cols + _CA_LF_SIZE) + _CA_NUL_SIZE;
}

static void _ca_scale(Ca_px_def *buf, const Chart *ca, uint16_t wid, uint16_t hgt)
{
	for (uint32_t y = 0; y < hgt; ++y)
		for (uint32_t x = 0; x < wid; ++x)
			buf[x + y * wid] = *ca_px(ca, x * ca->width/wid, y * ca->height/hgt);
}

/* Pixel to ASCII Art */
static int _ca_px2aa(char *iobuf, const Chart *ca, double rate)
{
	/* raw: " `'.-,^_:~;>/=)?LYfjTzVhPeH639EbAd8%MB@" */
	static const char ch_px[] = " `'.-,^_:~;>/=)?LYfjTzVhPeH639EbAd8%MBB@@";
	#define CH_PX_RANGE (sizeof ch_px - 2)

	if (iobuf == NULL || ca == NULL || rate <= 0)
		return -1;

	uint16_t wid = ca->cols;
	uint16_t hgt = (uint16_t) ((wid * ca->height) / (ca->width * 2 * rate));
	Ca_px_def *tmp_buf = NULL;

	if ((tmp_buf = calloc(wid * hgt, sizeof(Ca_px_def))) == NULL)
		return -1;
	_ca_scale(tmp_buf, ca, wid, hgt);

	for (uint32_t y = 0; y < hgt; ++y) {
		for (uint32_t x = 0; x < wid; ++x)
			*iobuf++ = ch_px[tmp_buf[x + y * wid] * CH_PX_RANGE / 255];
		*iobuf++ = '\n';
	}
	*iobuf = '\0';

	free(tmp_buf);
	return 0;
}
