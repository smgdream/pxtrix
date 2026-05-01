// SPDX-License-Identifier: MIT
/* Copyright (c) 2025 Smgdream */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "pgm.h"

Pgm *pgm_new(uint16_t wid, uint16_t hgt, uint16_t maxval)
{
	Pgm *p5 = NULL;

	/* uint16_t maxval > 65535 is impossible */
	if (maxval == 0)
		return NULL;
	if ((p5 = (Pgm *)malloc(sizeof(Pgm))) == NULL)
		return NULL;
	p5->buf = NULL;
	if (wid * hgt != 0
	 && (p5->buf = (Pgm_px_def *) calloc(wid * hgt, sizeof(Pgm_px_def))) == NULL) {
		free(p5);
		return NULL;
	}
	p5->width = wid;
	p5->height = hgt;
	p5->maxval = maxval;

	return p5;
}

int pgm_free(Pgm *p5)
{
	if (p5 == NULL)
		return 1;
	if (p5->buf != NULL)
		free(p5->buf);
	free(p5);
	return 0;
}

Pgm *pgm_read(const char *name)
{
	FILE *fp = NULL;
	Pgm *tmp = NULL;
	Byte_t *iobuf = NULL;
	uint8_t px_siz = 0;
	
	if (!pgm_valid(name))
		return NULL;
	if ((fp = fopen(name, "rb")) == NULL)
		return NULL;
	if ((tmp = (Pgm *) malloc(sizeof(Pgm))) == NULL) {
		fclose(fp);
		return NULL;
	}

	/* Read the file header */
	if (fscanf(fp, "P5 %hu %hu %hu\n", &tmp->width, &tmp->height, &tmp->maxval) != 3) {
		free(tmp);
		fclose(fp);
		return NULL;
	}
	//fgetc(fp); // debug
	px_siz = (tmp->maxval < 256) ? 1 : 2;

	/* New pixel buffer */
	tmp->buf = (Pgm_px_def *) malloc(tmp->width * tmp->height * sizeof(Pgm_px_def));
	if (tmp->buf == NULL) {
		free(tmp);
		fclose(fp);
		return NULL;
	}
	/* New io buffer (todo: memory align issue)*/
	iobuf = (Byte_t *) malloc(tmp->width * tmp->height * px_siz);
	if (iobuf == NULL) {
		pgm_free(tmp);
		fclose(fp);
		return NULL;
	}
	
	/* Read pixels (byte order BE) */
	if (fread(iobuf, px_siz, tmp->width * tmp->height, fp) != tmp->width * tmp->height) {
		free(iobuf);
		pgm_free(tmp);
		fclose(fp);
		return NULL;
	}
	pgm_decode(tmp->buf, iobuf, tmp->width * tmp->height, tmp->maxval);
	
	free(iobuf);
	fclose(fp);
	return tmp;
}

int pgm_write(const Pgm *p5, const char *name)
{
	FILE *fp = NULL;
	Byte_t *iobuf = NULL;
	uint8_t px_siz = 0;

	if (p5 == NULL || name == NULL)
		return -1;
	if (p5->width * p5->height == 0 || p5->buf == NULL)
		return -1;
	if ((fp = fopen(name, "wb")) == NULL)
		return -1;
	px_siz = (p5->maxval < 256) ? 1 : 2; // or maxval / 128
	if ((iobuf = malloc(p5->width * p5->height * px_siz)) == NULL) {
		fclose(fp);
		return -1;
	}

	pgm_encode(iobuf, p5->buf, p5->width * p5->height, p5->maxval);
	/* Write the header */
	fprintf(fp, "P5 %hu %hu %hu\n", p5->width, p5->height, p5->maxval);
	/* Write the pixels */
	fwrite(iobuf, p5->width * p5->height, px_siz, fp);

	free(iobuf);
	fclose(fp);
	return 0;
}

Pgm *pgm_set(Pgm *p5, uint16_t wid, uint16_t hgt, uint16_t maxval)
{
	/* uint16_t maxval > 65535 is impossible */
	if (p5 == NULL || maxval == 0)
		return NULL;

	if (wid * hgt == 0 && p5->buf != NULL) {
		free(p5->buf);
		p5->buf = NULL;
	}

	if (wid * hgt != 0 && wid * hgt != p5->width * p5->height) {
		Pgm_px_def *new_buf = NULL;

		if ((new_buf = calloc(1, wid * hgt * sizeof(Pgm_px_def))) == NULL)
			return NULL;
		if (p5->buf != NULL)
			free(p5->buf);
		p5->buf = new_buf;
	}
	p5->width = wid;
	p5->height = hgt;
	p5->maxval = maxval;

	return p5;
}

int pgm_info(const Pgm *p5, void *fileptr)
{
	FILE *fp = (FILE *) fileptr;

	if (p5 == NULL || fp == NULL)
		return -1;

	//fprintf(fp, "id:%s", "P5");
	fprintf(fp, "width:  %"PRIu16"\n", p5->width);
	fprintf(fp, "height: %"PRIu16"\n", p5->height);
	fprintf(fp, "maxval: %"PRIu16"\n", p5->maxval);

	return 0;
}

int pgm_valid(const char *name)
{
	FILE *fp = NULL;
	char tmp[3] = {'\0'};

	if (name == NULL || (fp = fopen(name, "rb")) == NULL)
		return 0;
	if (fread(tmp, 2, 1, fp) != 1) {
		fclose(fp);
		return 0;
	}
	fclose(fp);

	if (strcmp(tmp, "P5") == 0)
		return 1; /* valid */
	else
		return 0; /* invalid */
}