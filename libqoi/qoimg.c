/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#define __LIBQOI_INSIDE__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qoimg.h"
#include "bswap.h"

/* sizeof QOI_OP_RGBA */
#define QOI_MAX_PX_SIZE (5)

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
	Byte *px_iobuf = NULL;
	uint32_t wid = 0, hgt = 0;

	if ( name == NULL
	 || qoi_test(name) == 0
	 || (fp = fopen(name, "rb")) == NULL)
		return NULL;
	if ((qoi = (Qoimg *) malloc(sizeof(Qoimg))) == NULL) {
		fclose(fp);
		return NULL;
	}

	/* Read the qoi header */
	fread(&qoi->header, 14, 1, fp);
	qoi->header.width = bswap32(qoi->header.width);
	qoi->header.height = bswap32(qoi->header.height);
	
	wid = qoi->header.width;
	hgt = qoi->header.height;
	/* Alloc the space of px_iobuf and qoi pixel buffer */
	if ((px_iobuf = (Byte *) calloc(1, wid * hgt * QOI_MAX_PX_SIZE)) == NULL
	 || (qoi->buf = (Qoi_px_def *) calloc(1, wid * hgt * sizeof(Qoi_px_def))) == NULL) {
		if (qoi->buf != NULL)
			free(qoi->buf);
		if (px_iobuf != NULL)
			free(px_iobuf);
		free(qoi);
		fclose(fp);
		return NULL;
	}

	/* decode the date chunks */
	fread(px_iobuf, 1, wid * hgt * QOI_MAX_PX_SIZE, fp); // It ret the sizeof all the chunk
	qoi_decode(qoi->buf, px_iobuf, wid * hgt);
	
	free(px_iobuf);
	fclose(fp);
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
	if ((iobuf = (Byte *) malloc(wid * hgt * QOI_MAX_PX_SIZE)) == NULL) {
		fclose(fp);
		return 1;
	}
	
	tmph = qoi->header;
	tmph.width = bswap32(tmph.width);
	tmph.height = bswap32(tmph.height);
	fwrite(&tmph, 14, 1, fp); // write header

	datsiz = qoi_encode(iobuf, qoi->buf, wid * hgt); // encode
	fwrite(iobuf, datsiz, 1, fp); // write date chunks
	fwrite(&end_marker, 8, 1, fp); // write end-marker
	
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
	fprintf(fp, "%s: %.4s\n", "magic", qoi->header.magic);
	fprintf(fp, "%s: %u\n", "width", qoi->header.width);
	fprintf(fp, "%s: %u\n", "height", qoi->header.height);
	fprintf(fp, "%s: %u\n", "channels", qoi->header.channels);
	fprintf(fp, "%s: %u\n", "colorspace", qoi->header.colorspace);
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

int qoi_test(const char *name)
{
	FILE *fp = NULL;
	Qoimg qoi;

	if (name == NULL || (fp = fopen(name, "rb")) == NULL)
		return 0;
	if (fread(&qoi, 1, 14, fp) != 14) {
		fclose(fp);
		return 0;
	}
	fclose(fp);

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
