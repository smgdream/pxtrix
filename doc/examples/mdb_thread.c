/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream
 *
 * Name: Pxtrix
 * Author: smgdream
 * License: MIT Licnese
 * Version: 0.1.6
 * Date: 2024-11-12
 */

/* Not support for tcc */

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include "image.h"
#include "pic2img.h"
#include "bmpimg.h"
#include "qoimg.h"
#include "perf.h"

/* -2 <= real <= 2, -2 <= imag <= -2
 * nbl means: no belong */
int32_t nbl2mandelbrot(double real, double imag);
void *draw_the_mdb_line(void *);
static int max_it = 0;
static uint64_t img_end_ptr = 0;
static double gamm = 0;

struct targ {
	int id;
	Image *img;
};

int end_arr[64] = {0};

static inline int th_end(void)
{
	int cnt = 0;
	for (int i = 0; i < 64; ++i)
		cnt += end_arr[i];
	if (cnt == 64)
		return 1;
	else
		return 0;
}

int main(int argc, char **argv)
{
	pthread_t tid[64];
	Bmpimg *b = NULL;
	Image *i = NULL;
	Qoimg *q = NULL;

	if (argc != 3) {
		fprintf(stderr, "too %s argument\n", (argc > 3) ? "many" : "few");
		fprintf(stderr, "arg1: size of image; arg2: gamma\n");
		fprintf(stderr, "e.g. program 1920 0.3\n");
		return 1;
	}
	
	max_it = atoi(argv[1]);
	gamm = atof(argv[2]);
	if ((i = img_new(atoi(argv[1]), atoi(argv[1]), 24, sRGB)) == NULL)
		return 1;
	if ((b = bmp_new(BMP_EMPTY)) == NULL)
		return 2;
	if ((q = qoi_new(QOI_EMPTY)) == NULL)
		return 3;
	timer_start(); //////
	
	struct targ tp_arr[64];

	for (int it = 0; it < 64; ++it) {
		tp_arr[it].id = it;
		tp_arr[it].img = i;
	}
	for (int it = 0; it < 64; ++it)
		pthread_create(&tid[it], NULL, draw_the_mdb_line, (void *)&tp_arr[it]);
	while (1)
		if (th_end())
			break;
		else
			sleep(1);
	printf("Time of MDB draw: %.2f s\n", time_step() / 1000000.0); ///////////////

	//img2bmp(i, b);
	//bmp_write(b, "mdb.bmp");
	//printf("Time of bmp out: %u us\n", time_step()); /////////

	img2qoi(i, q);
	qoi_write(q, "mdb.qoi");
	printf("Time of qoi out: %u us\n", time_step()); /////////

	img_free(i);
	bmp_free(b);
	qoi_free(q);
	return 0;
}

void *draw_the_mdb_line(void *ptr)
{
	uint64_t tmp = 0;
	uint32_t line = ((struct targ *)ptr)->id;
	Image *img = ((struct targ *)ptr)->img;
	uint32_t len = max_it;

	int32_t x = 0, y = 0;
	for (;line < img->height; line += 64)
		img_for_px(x, y, 0, line, len, 1) {
			int32_t val = nbl2mandelbrot( (double)(x - ((int64_t)img->width >> 1)) * 4.0 / (double)img->width, (double)(y - ((int64_t)img->height >> 1)) * 4.0 / (double)img->height);
			uint8_t clr = (val < 0) ? 0 : (uint8_t)(pow((double)val / (double)max_it, gamm) * 255) ;
			img_px(img, x, y)->r = clr;
			img_px(img, x, y)->g = clr;
			img_px(img, x, y)->b = clr;
			img_px(img, x, y)->a = 255;
		}
	end_arr[((struct targ *)ptr)->id] = 1;
	return NULL;

}

#define MAX_IT (8192)

int32_t nbl2mandelbrot(double re, double im)
{
	complex z = 0 + 0 * I;
	complex c = re + im * I;
	
	for (int i = 0; i < max_it; ++i, z = z * z + c)
		if (cabs(z) > 2)
			return i;
	return -1;
}
