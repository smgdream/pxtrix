/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#define __LIBBMP_INSIDE__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bmpimg.h"

typedef uint8_t Byte;

int bmp_set_fileheader(Bmpfile_header *bfhp, int32_t wid, int32_t hgt)
{
	if (bfhp == NULL)
		return 1;
	bfhp->bf_type = BMP_TYPE_BM;
	bfhp->bf_size = (uint32_t)(14 + sizeof(Bmpinfo_header) + hgt * bmp_sizeof_line(wid));
	bfhp->bf_reserved1 = 0;
	bfhp->bf_reserved2 = 0;
	bfhp->bf_offbyte = (uint32_t)(14 + sizeof(Bmpinfo_header));
	return 0;
}

int bmp_set_infoheader(Bmpinfo_header *bihp, int32_t wid, int32_t hgt, uint16_t bpp)
{
	if (bihp == NULL)
		return 1;
	bihp->bi_size = (uint32_t) sizeof *bihp;
	bihp->bi_width = wid;
	bihp->bi_height = hgt;
	bihp->bi_planes = 1;
	bihp->bi_bitcount = bpp;
	bihp->bi_compression = 0;
	bihp->bi_sizeimage = (uint32_t)(hgt * bmp_sizeof_line(wid));
	bihp->bi_x_px_permeter = 0;
	bihp->bi_y_px_permeter = 0;
	bihp->bi_clrused = 0;
	bihp->bi_clrimportant = 0;
	return 0;
}

Bmpimg *bmp_set(Bmpimg *bmp, int32_t wid, int32_t hgt, uint16_t bpp)
{
	if (bmp == NULL)
		return NULL;
	/* Set to empty, E.g. wid * hgt == 0 */
	if (wid * hgt == 0 && bmp->buf != NULL) {
		free(bmp->buf);
		bmp->buf = NULL;
	}
	
	/* if size-old != size-new realloc the pixel buffer */
	if (bmp->bih.bi_width * bmp->bih.bi_height != wid * hgt && wid * hgt != 0) {
		Bmp_px_def *new_buf = NULL;

		new_buf = (Bmp_px_def *) calloc(1, wid * hgt * sizeof(Bmp_px_def));
		if (new_buf == NULL)
			return NULL;
		if (bmp->buf != NULL)
			free(bmp->buf);
		bmp->buf = new_buf;
	}

	bmp_set_fileheader(&bmp->bfh, wid, hgt);
	bmp_set_infoheader(&bmp->bih, wid, hgt, bpp);
	return bmp;
}

Bmpimg *bmp_new(int32_t wid, int32_t hgt, uint16_t bpp)
{
	Bmpimg *bmp = NULL;

	/* new a Bmpimg object */
	if ((bmp = (Bmpimg *) malloc(sizeof(Bmpimg))) == NULL)
		return NULL;

	/* new a bmp pixel buffer, or set buf = NULL if wid*hgt == 0 */
	if (wid * hgt != 0) {
		bmp->buf = (Bmp_px_def *) calloc(1, wid * hgt * sizeof(Bmp_px_def));
		if (bmp->buf == NULL) {
			free(bmp);
			return NULL;
		}
	} else
		bmp->buf = NULL;
	
	bmp_set_fileheader(&bmp->bfh, wid, hgt);
	bmp_set_infoheader(&bmp->bih, wid, hgt, bpp);
	return bmp;
}

int bmp_free(Bmpimg *bmp)
{
	if (bmp == NULL)
		return 1;
	if (bmp->buf != NULL)
		free(bmp->buf);
	free(bmp);

	return 0;
}

Bmpimg *bmp_read(const char *name)
{
	Bmpimg *bmp = NULL;
	Bmp_px_def *px_ptr_dest = NULL;
	FILE *fp = NULL;
	Byte *px_iobuf = NULL, *px_ptr_src = NULL;
	int32_t wid = 0, hgt = 0;
	
	/* New a Bmpimg object */
	if ((bmp = (Bmpimg *) malloc(sizeof(Bmpimg))) == NULL)
		return NULL;
	if (bmp_test(name) == 0
	 || (fp = fopen(name, "rb")) == NULL) {
		free(bmp);
		return NULL;
	}

	/* Read bitmap file header */
	fread(&bmp->bfh.bf_type,		sizeof bmp->bfh.bf_type,		1, fp);
	fread(&bmp->bfh.bf_size,		sizeof bmp->bfh.bf_size,		1, fp);
	fread(&bmp->bfh.bf_reserved1,	sizeof bmp->bfh.bf_reserved1,	1, fp);
	fread(&bmp->bfh.bf_reserved2,	sizeof bmp->bfh.bf_reserved2,	1, fp);
	fread(&bmp->bfh.bf_offbyte,		sizeof bmp->bfh.bf_offbyte,		1, fp);
	/* Read bitmap info header */
	fread(&bmp->bih, sizeof bmp->bih, 1, fp);
	
	wid = bmp->bih.bi_width;
	hgt = bmp->bih.bi_height;
	/* Init Bmp pixel buffer*/
	if ((bmp->buf = (Bmp_px_def *) malloc(wid * hgt * sizeof(Bmp_px_def))) == NULL
	 || (px_iobuf = (Byte *) malloc(hgt * bmp_sizeof_line(wid))) == NULL) {
		if (px_iobuf != NULL)
			free(px_iobuf);
		if (bmp->buf != NULL)
			free(bmp->buf);
		free(bmp);
		fclose(fp);
		return NULL;
	}

	/* Read pixels row by row */
	px_ptr_src = px_iobuf;
	px_ptr_dest = bmp->buf;	 
	fread(px_iobuf, hgt * bmp_sizeof_line(wid), 1, fp);
	for (int i = 0; i < hgt; ++i) {
		memcpy(px_ptr_dest, px_ptr_src, wid * sizeof(Bmp_px_def));
		px_ptr_src += bmp_sizeof_line(wid);
		px_ptr_dest += wid;
	}

	free(px_iobuf);
	fclose(fp);
	return bmp;
}

int bmp_write(const Bmpimg *bmp, const char *name)
{
	Bmp_px_def *px_ptr_src = NULL;
	Byte *px_iobuf = NULL, *px_ptr_dest = NULL;
	FILE *fp = NULL;
	int32_t wid = 0, hgt = 0;

	if (bmp == NULL
	 || bmp->buf == NULL
	 || name == NULL
	 || (fp = fopen(name, "wb")) == NULL)
		return 1;
	
	/* Write bitmap file header */
	fwrite(&bmp->bfh.bf_type,		sizeof bmp->bfh.bf_type,		1, fp);
	fwrite(&bmp->bfh.bf_size,		sizeof bmp->bfh.bf_size,		1, fp);
	fwrite(&bmp->bfh.bf_reserved1,	sizeof bmp->bfh.bf_reserved1,	1, fp);
	fwrite(&bmp->bfh.bf_reserved2,	sizeof bmp->bfh.bf_reserved2,	1, fp);
	fwrite(&bmp->bfh.bf_offbyte,	sizeof bmp->bfh.bf_offbyte,		1, fp);
	/* write bitmap info header */
	fwrite(&bmp->bih, sizeof bmp->bih, 1, fp);

	wid = bmp->bih.bi_width;
	hgt = bmp->bih.bi_height;
	/* Because the 0x00 used to align at the end of the line, so we should use calloc */
	if ((px_iobuf = (Byte *) calloc(1, hgt * bmp_sizeof_line(wid))) == NULL) {
		fclose(fp);
		return 1;
	}

	/* Write the pixels row by row */
	px_ptr_src = bmp->buf;
	px_ptr_dest = px_iobuf;
	for (int i = 0; i < hgt; ++i) {
		memcpy(px_ptr_dest, px_ptr_src, wid * sizeof(Bmp_px_def));
		px_ptr_src += wid;
		px_ptr_dest += bmp_sizeof_line(wid);
	}
	fwrite(px_iobuf, hgt * bmp_sizeof_line(wid), 1, fp);

	free(px_iobuf);
	fclose(fp);
	return 0;
}

int bmp_info(const Bmpimg *bmp, void *fptr)
{
	FILE *fp = (FILE *) fptr;

	if (bmp == NULL || fp == NULL)
		return 1;
	
	fprintf(fp, "\nBitmap file header:\n");
	fprintf(fp, "%s: %c%c\n",	"bf_type",		*((char *)&bmp->bfh.bf_type + 0), *((char *)&bmp->bfh.bf_type + 1));
	fprintf(fp, "%s: %u\n",		"bf_size",		bmp->bfh.bf_size);
	fprintf(fp, "%s: %hu\n",	"bf_reserved1",	bmp->bfh.bf_reserved1);
	fprintf(fp, "%s: %hu\n",	"bf_reserved2",	bmp->bfh.bf_reserved2);
	fprintf(fp, "%s: %u\n",		"bf_offbits",	bmp->bfh.bf_offbyte);
	
	fprintf(fp, "\nBitmap info header:\n");
	fprintf(fp, "%s: %u\n",	"bi_size",			bmp->bih.bi_size);
	fprintf(fp, "%s: %d\n",	"bi_width",			bmp->bih.bi_width);
	fprintf(fp, "%s: %d\n",	"bi_height",		bmp->bih.bi_height);
	fprintf(fp, "%s: %hu\n","bi_planes",		bmp->bih.bi_planes);
	fprintf(fp, "%s: %hu\n","bi_bitcount",		bmp->bih.bi_bitcount);
	fprintf(fp, "%s: %u\n",	"bi_compression",	bmp->bih.bi_compression);
	fprintf(fp, "%s: %u\n",	"bi_sizeimage",		bmp->bih.bi_sizeimage);
	fprintf(fp, "%s: %d\n",	"bi_x_px_per_meter",bmp->bih.bi_x_px_permeter);
	fprintf(fp, "%s: %d\n",	"bi_y_px_per_meter",bmp->bih.bi_y_px_permeter);
	fprintf(fp, "%s: %u\n",	"bi_clrused",		bmp->bih.bi_clrused);
	fprintf(fp, "%s: %u\n",	"bi_clrimportant",	bmp->bih.bi_clrimportant);
	return 0;
}

int bmp_test(const char *name)
{
	FILE *fp = NULL;
	uint16_t tmp = 0;

	if (name == NULL || (fp = fopen(name, "rb")) == NULL)
		return 0;
	if (fread(&tmp, 1, 2, fp) != 2) {
		fclose(fp);
		return 0;
	}
	fclose(fp);

	if (tmp == BMP_TYPE_BM)
		return 1; // valid bmp file
	else
		return 0; // invalid bmp file
}
