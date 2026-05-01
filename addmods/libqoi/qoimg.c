// SPDX-License-Identifier: MIT
/* Copyright (c) 2024 Smgdream */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "qoimg.h"
#include "bswap.h"

/* sizeof QOI_OP_RGBA */
#define PX_SIZE_MAX 5
#define PX_SIZE_MIN (1.0/62.0)
#define QOI_HEADER_SIZE 14
#define QOI_ENDER_SIZE 8

Qoimg *qoi_new(uint32_t wid, uint32_t hgt, uint8_t channels, uint8_t colorspace)
{
	Qoimg *qoi = NULL;

	if ((qoi = (Qoimg *) malloc(sizeof(Qoimg))) == NULL)
		return NULL;
	
	qoi->buf = NULL;
	if (wid * hgt != 0
	 && (qoi->buf = (Qoi_px_def *) calloc(1, wid * hgt * sizeof(Qoi_px_def))) == NULL) {
		free(qoi);
		return NULL;
	}
	
	memcpy(qoi->header.magic, "qoif", 4);
	qoi->header.width = wid;
	qoi->header.height = hgt;
	qoi->header.channels = channels;
	qoi->header.colorspace = colorspace;
	return qoi;
}

int qoi_free(Qoimg *qoi)
{
	if (qoi == NULL)
		return 1;
	if (qoi->buf != NULL)
		free(qoi->buf);
	free(qoi);

	return 0;
}

Qoimg *qoi_read(const char *name)
{
	Qoimg *qoi = NULL;
	FILE *fp = NULL;

	if (name == NULL
	 || !qoi_valid(name)
	 || (fp = fopen(name, "rb")) == NULL)
		return NULL;

	/* If qoi_read_stream return NULL. This function 
	 * will also return NULL in the end.  */
	qoi = qoi_read_stream(fp);

	fclose(fp);
	return qoi;
}

Qoimg *qoi_read_stream(void *fileptr)
{
	Qoimg *qoi = NULL;
	Byte *px_iobuf = NULL;
	FILE *stream = (FILE *) fileptr;
	uint32_t wid = 0, hgt = 0;
	
	/* test the qoi stream valid or not? by qoi_vavlid_stream */
	if (!qoi_valid_stream(stream))
		return NULL;

	if ((qoi = (Qoimg *) malloc(sizeof(Qoimg))) == NULL)
		return NULL;

	/* Read the qoi header */
	if (fread(&qoi->header, QOI_HEADER_SIZE, 1, stream) != 1) {
		free(qoi);
		return NULL;
	}
	qoi->header.width = bswap32(qoi->header.width);
	qoi->header.height = bswap32(qoi->header.height);
	
	wid = qoi->header.width;
	hgt = qoi->header.height;
	/* Alloc the space of px_iobuf and qoi pixel buffer */
	if ((px_iobuf = (Byte *) calloc(1, wid * hgt * PX_SIZE_MAX)) == NULL
	 || (qoi->buf = (Qoi_px_def *) calloc(1, wid * hgt * sizeof(Qoi_px_def))) == NULL) {
		if (qoi->buf != NULL)
			free(qoi->buf);
		if (px_iobuf != NULL)
			free(px_iobuf);
		free(qoi);
		return NULL;
	}

	/* Read the qoi date chunk. It ret the sizeof all the chunk */
	if (fread(px_iobuf, 1, wid * hgt * PX_SIZE_MAX, stream) < wid * hgt * PX_SIZE_MIN) {
		free(px_iobuf);
		qoi_free(qoi);
		return NULL;
	}
	/* decode the date chunks */
	qoi_decode(qoi->buf, px_iobuf, wid * hgt);
	
	/* Do not close the stream */
	free(px_iobuf);
	return qoi;
}

int qoi_write(const Qoimg *qoi, const char *name)
{
	FILE *fp = NULL;
	Byte *iobuf = NULL;
	uint32_t wid = 0, hgt = 0;
	size_t datsiz = 0;
	Qoi_header tmph;
	const uint64_t end_marker = 0x0100000000000000;

	if (qoi == NULL || qoi->buf == NULL || name == NULL
	 || (fp = fopen(name, "wb")) == NULL)
		return 1;
	
	wid = qoi->header.width;
	hgt = qoi->header.height;
	if ((iobuf = (Byte *) malloc(wid * hgt * PX_SIZE_MAX)) == NULL) {
		fclose(fp);
		return 1;
	}
	
	tmph = qoi->header;
	tmph.width = bswap32(tmph.width);
	tmph.height = bswap32(tmph.height);
	/* encode */
	datsiz = qoi_encode(iobuf, qoi->buf, wid * hgt);

	fwrite(&tmph, QOI_HEADER_SIZE, 1, fp); // write header
	fwrite(iobuf, datsiz, 1, fp); // write date chunks
	fwrite(&end_marker, QOI_ENDER_SIZE, 1, fp); // write end-marker
	
	free(iobuf);
	fclose(fp);
	return 0;
}

int qoi_info(const Qoimg *qoi, void *fptr)
{
	FILE *fp = (FILE *) fptr;

	if (qoi == NULL || fp == NULL)
		return 1;

	fprintf(fp, "QOI header:\n");
	fprintf(fp, "%s: %.4s\n",		"magic",		qoi->header.magic);
	fprintf(fp, "%s: %"PRIu32"\n",	"width",		qoi->header.width);
	fprintf(fp, "%s: %"PRIu32"\n",	"height",		qoi->header.height);
	fprintf(fp, "%s: %"PRIu8" (%s)\n",	"channels", qoi->header.channels,
								(qoi->header.channels == QOI_CH_RGBA) ? 
								"RGBA" : "RGB");
	fprintf(fp, "%s: %"PRIu8" (%s)\n", "colorspace",qoi->header.colorspace,
						(qoi->header.colorspace == QOI_GAMUT_SRGB_LA) ?
						"sRGB with linear alpha" : "all channels linear");
	return 0;
}

Qoimg *qoi_set(Qoimg *qoi, uint32_t wid, uint32_t hgt, uint8_t channels, uint8_t colorspace)
{
	if (qoi == NULL)
		return NULL;

	/* When wid * hgt == 0, set buffer as empty. */
	if (wid * hgt == 0 && qoi->buf != NULL) {
		free(qoi->buf);
		qoi->buf = NULL;
	}
	
	/* Allocate a new buffer. */
	if (qoi->header.width * qoi->header.height != wid * hgt && wid * hgt != 0) {
		Qoi_px_def *new_buf = NULL;

		if ((new_buf = (Qoi_px_def *) calloc(1, wid * hgt * sizeof(Qoi_px_def))) == NULL)
			return NULL;
		if (qoi->buf != NULL)
			free(qoi->buf);
		qoi->buf = new_buf;
	}

	qoi->header.width = wid;
	qoi->header.height = hgt;
	qoi->header.channels = channels;
	qoi->header.colorspace = colorspace;
	return qoi;
}

int qoi_valid(const char *name)
{
	FILE *fp = NULL;
	int ret = 0;

	if (name == NULL || (fp = fopen(name, "rb")) == NULL)
		return 0;
	ret = qoi_valid_stream(fp);
	fclose(fp);
	return ret;
}

int qoi_valid_stream(void *stream)
{
	FILE *fp = (FILE *)stream;
	fpos_t fpos;
	Qoimg qoi;

	if (fp == NULL)
		return 0;
	fgetpos(fp, &fpos);

	if (fread(&qoi, QOI_HEADER_SIZE, 1, fp) != 1) {
		fsetpos(fp, &fpos);
		return 0;
	}
	fsetpos(fp, &fpos);

	if (strncmp(qoi.header.magic, "qoif", 4) != 0
	 || qoi.header.width < 1
	 || qoi.header.height < 1
	 || qoi.header.channels < 3
	 || qoi.header.channels > 4
	 || qoi.header.colorspace > 1)
		return 0; // invalid QOI file
	else
		return 1; // valid QOI file
	
}
