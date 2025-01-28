/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

#define __LIBPGM_INSIDE__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm.h"

Pgm *pgm_new(uint16_t wid, uint16_t hgt)
{
	Pgm *p5 = NULL;

	if ((p5 = (Pgm *)malloc(sizeof(Pgm))) == NULL)
		return NULL;
	p5->buf = NULL;
	if (wid * hgt != 0
	 && (p5->buf = (Pgm_px_def *)calloc(1, wid * hgt)) == NULL) {
		free(p5);
		return NULL;
	}
	p5->width = wid;
	p5->height = hgt;
	p5->maxval = 255;

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
	
	if (!pgm_test(name))
		return NULL;
	if ((fp = fopen(name, "rb")) == NULL)
		return NULL;
	if ((tmp = (Pgm *) malloc(sizeof(Pgm))) == NULL) {
		fclose(fp);
		return NULL;
	}

	/* Read the file header */
	fscanf(fp, "P5 %hu %hu %hu", &tmp->width, &tmp->height, &tmp->maxval);
	/* New buffer */
	tmp->buf = (Pgm_px_def *) malloc(tmp->width * tmp->height * sizeof(Pgm_px_def));
	if (tmp->buf == NULL) {
		free(tmp);
		fclose(fp);
		return NULL;
	}
	fgetc(fp); // debug
	/* Read pixels */
	fread(tmp->buf, tmp->width * tmp->height * sizeof(Pgm_px_def), 1, fp);
	
	fclose(fp);
	return tmp;
}

int pgm_write(const Pgm *p5, const char *name)
{
	FILE *fp = NULL;

	if (p5->width * p5->height == 0 || p5->buf == NULL)
		return -1;
	if ((fp = fopen(name, "wb")) == NULL)
		return -1;

	/* Write the header */
	fprintf(fp, "P5 %hu %hu %hu\n", p5->width, p5->height, p5->maxval);
	/* Write the pixels */
	fwrite(p5->buf, p5->width * p5->height * sizeof(Pgm_px_def), 1, fp);

	fclose(fp);
	return 0;
}

Pgm *pgm_set(Pgm *p5, uint16_t wid, uint16_t hgt)
{
	if (wid * hgt == 0) {
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

	return p5;
}

int pgm_info(const Pgm *p5, void *fileptr)
{
	FILE *fp = (FILE *) fileptr;

	if (p5 == NULL)
		return -1;

	//fprintf(fp, "id:%s", "P5");
	fprintf(fp, "width:  %d\n", p5->width);
	fprintf(fp, "height: %d\n", p5->height);
	fprintf(fp, "maxval: %d\n", p5->maxval);

	return 0;
}

int pgm_test(const char *name)
{
	FILE *fp = NULL;
	char tmp[3] = {'\0'};

	if ((fp = fopen(name, "rb")) == NULL)
		return 0;
	fread(tmp, 2, 1, fp);

	if (strcmp(tmp, "P5") == 0)
		return 1;
	else
		return 0;
}