/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "chart.h"


static size_t __ca_iobuf_size(const Chart *ca);
static uint8_t *__ca_scale(const Chart *src, uint8_t *dest, uint16_t wid, uint16_t hgt);
static int __ca_px2cart(const Chart *ca, char *iobuf, double rate);


Chart *ca_new(uint16_t wid, uint16_t hgt, uint16_t cols)
{
	Chart *tmp = NULL;
	
	if ((tmp = malloc(sizeof(Chart))) == NULL)
		return NULL;
		
	tmp->buf = NULL;
	if ((tmp->buf = calloc(wid * hgt, sizeof(Ca_px_def))) == NULL) {
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

/* The argument rate is scale rate of width (valid range (0, 2]). */
int ca_write(const Chart *ca, void *fptr, double rate)
{
	char *iobuf = NULL;
	FILE *fp = (FILE *) fptr;
	
	if (ca == NULL || fp == NULL)
		return -1;

	if ((iobuf = calloc(1, __ca_iobuf_size(ca))) == NULL)
		return -1;
	__ca_px2cart(ca, iobuf, rate);
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

		if ((new_buf = calloc(1, wid * hgt * sizeof(Ca_px_def))) == NULL)
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

	fprintf(fp, "width:\t%"		PRIu16,	ca->width);
	fprintf(fp, "height:\t%"	PRIu16,	ca->height);
	fprintf(fp, "columns:\t%"	PRIu16,	ca->cols);
	return 0;
}

#define CHAR_PER_UNIT (2)
#define MAX_RATE (2)
#define CH_LF_SIZ (sizeof((char)'\n'))
#define CH_NUL_SIZ (sizeof((char)'\0'))
static size_t __ca_iobuf_size(const Chart *ca)
{
	size_t rows = 0;
	
	rows = ca->height / (ca->width/ca->cols);
	return rows * (ca->cols * CHAR_PER_UNIT * MAX_RATE + CH_LF_SIZ) + CH_NUL_SIZ;
}

static uint8_t *__ca_scale(const Chart *src, uint8_t *dest, uint16_t wid, uint16_t hgt)
{
	uint8_t gris = 0;
	
	if (src == NULL || src->buf == NULL || dest == NULL)
		return NULL;

	/* vector scalar multiplication */
	for (uint16_t y = 0; y < src->height; ++y)
		for (uint16_t x = 0; x < src->width; ++x) {
			gris = 0.300 * ca_px(src, x, y)->r
				 + 0.590 * ca_px(src, x, y)->g
				 + 0.110 * ca_px(src, x, y)->b;
			dest[(uint32_t) ((double)wid/src->width  * x) +
				 (uint32_t) ((double)hgt/src->height * y) * wid
				] = gris;
		}
	return dest;
}

static int __ca_px2cart(const Chart *ca, char *buf, double rate)
{	
	char c = '\0';
	uint8_t *tmp_buf = NULL;
	uint16_t wid = 0, hgt = 0;
	size_t i = 0;
	/* Do not change the type of ch_list as char * (unless you know why). */
	static const char ch_list[] = " .-,:;^+~*wfacbd%%&$#@";

	if (ca == NULL || buf == NULL)
		return -1;
	
	wid = ca->cols;
	hgt = wid * ca->height / ca->width;
	wid *= rate;
	if ((tmp_buf = calloc(wid * hgt, sizeof(uint8_t))) == NULL)
		return -1;
	__ca_scale(ca, tmp_buf, wid, hgt);

	for (uint16_t y = 0; y < hgt; ++y) {
		for (uint16_t x = 0; x < wid; ++x) {
			uint8_t val = tmp_buf[x + y * wid];
			c = ch_list[val * (sizeof(ch_list)-1)/256];
			buf[i++] = c;
			buf[i++] = c;
		}
		buf[i++] = '\n';
	}
	buf[i] = '\0';

	free(tmp_buf);
	return 0;
}