/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream
 *
 * Name: Pxtrix
 * Author: smgdream
 * License: MIT Licnese
 * Date: 2026-02-22
 */

/* support for tcc
 * just for linux
 */
// system setting:
// usermod -a -G video <YOURUSER>
// compile:
// make lib
// gcc -Wall -Wextra -O3 -Ibuild/include -o mand.run main.c ctl.c build/perf.c build/lib/*.a -lm -lpthread
 
/* Example name: mandelbrot set viewer
 * Control:
 * exit: t
 * up: w
 * down: s
 * left: a
 * right:d
 * zoom in: =
 * zoom out: -
 * incerasing the number of iterations: ]
 * decreasing the number of iterations: [
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <pthread.h>
#include "image.h"
#include "fbdev.h"
#include "pic2img.h"
#include "render.h"
#include "perf.h"
#include "luts/lut_magma.h"
#include "ctl.h"

static unsigned max_it = 256;
static double gamma_val = 0.0;

typedef struct complex_t {
	double re;
	double im;
} Complex_t;

static inline Complex_t cadd(Complex_t a, Complex_t b)
{
	Complex_t y  = {0, 0};

	y.re = a.re + b.re;
	y.im = a.im + b.im;

	return y;
}

static inline Complex_t cmul(Complex_t a, Complex_t b)
{
	Complex_t y = {0, 0};

#ifdef FP_FAST_FMA
	y.re = fma(a.re, b.re , -(a.im * b.im));
	y.im = fma(a.re, b.im, a.im * b.re);
#else
	y.re = a.re * b.re - a.im * b.im;
	y.im = a.re * b.im + a.im * b.re;
#endif

	return y;
}

static inline double cabso2(Complex_t x)
{
#ifdef FP_FAST_FMA
	return fma(x.re, x.re, x.im * x.im);
#else
	return x.re * x.re + x.im * x.im;
#endif
}

/* -2 <= real <= 2, -2 <= imag <= -2
 * nbl means: no belong */
int32_t nbl2mandelbrot(Complex_t c);
int draw_mandelbrot(Image *img, const Complex_t *cam_pos, double scaler);

int main(int argc, char **argv)
{
	Fbdev *fb = NULL;
	Image *i = NULL;

	double scaler = 0.001;
	Complex_t camera_pos = {0, 0};
	 
	if (argc != 4) {
		fprintf(stderr, "too %s arguments\n", (argc > 4) ? "many" : "few");
		fprintf(stderr, "arg1: width of screen, arg2: height of screen, arg3: gamma\n");
		fprintf(stderr, "e.g. program 1920 1080 0.3\n");
		return 1;
	}

	if ((i = img_new(atoi(argv[1]), atoi(argv[2]), 24, sRGB)) == NULL)
		return 1;
	if ( (fb = fb_new(i->width, i->height, FB_DEVICE)) == NULL)
		return 2;
	gamma_val = atof(argv[3]);

	timer_start(); //////

	char kbact = '\0';
	for (int exit_flag = 0 ; exit_flag == 0;) {
		kbact = '\0';
		if (kbhit())
			kbact = getch();

		switch (kbact) {
		case 'w':
			camera_pos.im += scaler * 10;
			break;
		case 's':
			camera_pos.im -= scaler * 10;
			break;
		case 'a':
			camera_pos.re -= scaler * 10;
			break;
		case 'd':
			camera_pos.re += scaler * 10;
			break;
		case '-':
			scaler *= 1.25892;
			break;
		case '=':
			scaler /= 1.25892;
			break;
		case '[':
			max_it /= (max_it > 1) ? 2 : 1;
			break;
		case ']':
			max_it *= 2;
			break;
		case 't':
			exit_flag = 1;
			break;
		}
		draw_mandelbrot(i, &camera_pos, scaler);
		img2fb(i, fb);

		fb_write(fb, "/dev/fb0");
		fprintf(stderr, "FPS: %d\n", (int) (1000000.0 / time_step()));
	}

	img_free(i);
	fb_free(fb);
	return 0;
}

#define THREAD_NUM 64

struct dml {
	Image *img;
	const Complex_t *cam;
	double scaler;
	uint32_t ym; // y_mod
};

int draw_mandelbrot(Image *img, const Complex_t *cam_pos, double scaler)
{
	void *tcall_draw_mandelbrot_line(void *ptr);

	Px_def green = {0, 255, 0, 255};

	/* thread init */
	pthread_t tvec[THREAD_NUM] = {0};
	struct dml dml_vec[THREAD_NUM];
	void *tret[THREAD_NUM] = {0};

	for (int i = 0; i < THREAD_NUM; ++i) {
		dml_vec[i].img = img;
		dml_vec[i].cam = cam_pos;
		dml_vec[i].scaler = scaler;
		dml_vec[i].ym = i;
	}

	/* thread start */
	for (int tid = 0; tid < THREAD_NUM; ++tid)
		pthread_create(&tvec[tid], NULL, tcall_draw_mandelbrot_line, &dml_vec[tid]);
	/* thread wait */
	for (int tid = 0; tid < THREAD_NUM; ++tid)
		pthread_join(tvec[tid], &tret[tid]);
	/* thread end */

	*img_px(img, img->width/2, img->height/2) = green;
	return 0;
}

void *tcall_draw_mandelbrot_line(void *stu_ptr)
{
	int draw_mandelbrot_line(Image *img, const Complex_t *cam_pos, double scaler, uint32_t y);

	struct dml *ptr = (struct dml *) stu_ptr;
	draw_mandelbrot_line(ptr->img, ptr->cam, ptr->scaler, ptr->ym);

	pthread_exit((void *)0);

	return NULL;
}

int draw_mandelbrot_line(Image *img,
						 const Complex_t *cam_pos,
						 double scaler,
						 uint32_t ym)
{
	Complex_t c = {0, 0};
	int32_t val = 0;
	size_t inx = 0;
	uint32_t wid = img->width;
	uint32_t hgt = img->height;

	for (uint32_t y = ym; y < hgt; y += THREAD_NUM) {
		uint32_t x_it = 0, y_it = 0;
		img_for_px(x_it, y_it, 0, y, wid, 1) {
			c.re = cam_pos->re + ((int32_t)x_it - (int32_t)wid / 2) * scaler;
			c.im = cam_pos->im + ((int32_t)y_it - (int32_t)hgt / 2) * scaler;
			val = nbl2mandelbrot(c);
			inx = (size_t) (gamma_cx(gamma_val, (double) val / max_it) * 256);
			*img_px(img, x_it, y_it) = clr_lut_magma[inx];
		}
	}

	return 0;
}

int32_t nbl2mandelbrot(Complex_t c)
{
	Complex_t z = {0, 0};
	 
	for (unsigned i = 0; i < max_it; ++i, z = cadd(cmul(z, z), c))
		if (cabso2(z) > 2.0 * 2.0)
			return i;
	return 0;
}