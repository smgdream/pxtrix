// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

/* Framebuffer clock */
/* Different std c version have different result */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "image.h"
#include "fbdev.h"
#include "qoimg.h"
#include "pic2img.h"
#include "render.h"
#include "util.h"


#ifdef __linux__

#define SECS_PER_CIRCLE  60
#define MINS_PER_CIRCLE  60
#define HOURS_PER_CIRCLE 12

#define RI 0.4629
#define RO 0.4740
#define H_LEN 0.2083
#define M_LEN 0.2870
#define S_LEN 0.3694


#define FRAMEBUFFER_DEVICE "/dev/fb0"

Image *layout(Image *dest, const Image *src);
void get_hms(double *h, double *m, double *s);
int64_t get_ms(void);

static inline Px_hsva hsva_4val(float h, float s, float v, float a)
{
	Px_hsva px = {h, s, v, a};
	return px;
}

static inline Px_def *img_px_cent_vec2(const Image *img, Vec2 p)
{
	return img_px_c(img, p.x, p.y);
}

static inline double range_0to2pi(double rad)
{
	return (rad >= 0 && rad < TAU) ? rad :
		(rad >= TAU) ? rad - TAU : rad + TAU;  
}

int main(void)
{
	#if __STDC_VERSION__ < 202311L
		printf("Different std c version have different result.\n");
		msleep(3000);
	#endif
	/* get display resolution */
	uint32_t dis_wid = 0, dis_hgt = 0;
	if (fb_display_info(FRAMEBUFFER_DEVICE, &dis_wid, &dis_hgt) != 0) {
		fprintf(stderr, "error can't not get dispaly information from "
						"framebuffer device %s\n", FRAMEBUFFER_DEVICE);
		return -1;
	}

	Fbdev *out = fb_new(dis_wid, dis_hgt, FB_DEVICE);
	FILE *fb_stream = NULL;
#if (__STDC_VERSION__ >= 202311L)
#endif
#if (__STDC_VERSION__ >= 202311L)
	FILE *bg_fp = NULL;
#endif

	// background0 + background1 -> lay0
	// lay0 + ring -> lay_static
	// lay_static + lay_pointer -> fb_bitmap
	Image *bg0   = img_new(dis_wid, dis_hgt, 32, SRGB);
	Image *bg1	 = img_new(dis_wid, dis_hgt, 32, SRGB);
	Image *lay0  = img_new(dis_wid, dis_hgt, 32, SRGB);
	Image *ring  = img_new(dis_wid, dis_hgt, 32, SRGB);
	Image *lay_s = img_new(dis_wid, dis_hgt, 32, SRGB);
	Image *layp  = img_new(dis_wid, dis_hgt, 32, SRGB);
	Image *bitmap= img_new(dis_wid, dis_hgt, 32, SRGB);

	if ((fb_stream = fopen(FRAMEBUFFER_DEVICE, "wb")) == NULL)
		return -1;
#if (__STDC_VERSION__ >= 202311L)
	Qoimg *background = NULL;
	#warning Download bg024.qoi from github.com/smgdream/bed/blob/main/pxtrix/bg024.qoi
	/* Load the embed background qoi image */
	static const uint8_t bg_img[] = {
		#embed "bg024.qoi"
	};
	bg_fp = byteseq2tmpfile(bg_img, sizeof bg_img);
	background = qoi_read_stream(bg_fp);
	fclose(bg_fp);
	qoi2img(background, bg0);
#else
	fill(bg0, rgba_hex(0xffffffff));
#endif
	
	fill(bg1, rgba_hex(0x000000c0));
	scale(bg0, dis_wid, dis_hgt);
	layout(lay0, bg0);
	layout(lay0, bg1);

	/* sec should ptr < 450 */
	int ri = RI * min(dis_wid, dis_hgt), ro = RO * min(dis_wid, dis_hgt); 
	int32_t x = 0, y = 0;
	int32_t x_beg = -(int32_t) ring->width/2, wid = (int32_t)ring->width;
	int32_t y_beg = -(int32_t) ring->height/2, hgt = (int32_t)ring->height;

	img_for_px(x, y, x_beg, y_beg, wid, hgt)
		if (ri*ri <= x*x + y*y && x*x + y*y <= ro*ro) {
			Px_hsva px = {360 * polar_t(x, y) / TAU, 1, 1, 1};
			*img_px_c(ring, x, y) = hsva2rgba(px);
		} else {
			Px_hsva px = {0, 0, 0, 0};
			*img_px_c(ring, x, y) = hsva2rgba(px);
		}

	layout(lay_s, lay0);
	layout(lay_s, ring);
	
	double rh = H_LEN * min(dis_wid, dis_hgt);
	double rm = M_LEN * min(dis_wid, dis_hgt);
	double rs = S_LEN * min(dis_wid, dis_hgt);
	for (;; msleep(5)) {
		fill(layp, rgba_hex(0x00000000));
		double h = 0, m = 0, s = 0;
		get_hms(&h, &m, &s);

		double th = PI/2 - h/HOURS_PER_CIRCLE * TAU;
		double tm = PI/2 - m/MINS_PER_CIRCLE  * TAU;
		double ts = PI/2 - s/SECS_PER_CIRCLE  * TAU;

		double hh = range_0to2pi(th + 5*PI/3);
		double hm = range_0to2pi(tm + 1*PI/3);
		double hs = range_0to2pi(ts + 3*PI/3);
		
		Vec2 po = {0, 0};
		Vec2 ph = {rh*cos(th), rh*sin(th)};
		Vec2 pm = {rm*cos(tm), rm*sin(tm)};
		Vec2 ps = {rs*cos(ts), rs*sin(ts)};

		for (double t = 0; t <= 1; t += 1/(2*rs)) {
			*img_px_cent_vec2(layp, vec2_lerp(po, ph, t)) = hsva2rgba(hsva_4val(hh / TAU * 360.0f, 1, 1, 1));
			*img_px_cent_vec2(layp, vec2_lerp(po, pm, t)) = hsva2rgba(hsva_4val(hm / TAU * 360.0f, 1, 1, 1));
			*img_px_cent_vec2(layp, vec2_lerp(po, ps, t)) = hsva2rgba(hsva_4val(hs / TAU * 360.0f, 1, 1, 1));
		}

		layout(bitmap, lay_s);
		layout(bitmap, layp);

		img2fb(bitmap, out);
		fb_write_stream(out, fb_stream);
	}
	
	img_free(bg0);
	img_free(bg1);
	img_free(lay0);
	img_free(ring);
	img_free(lay_s);
	img_free(layp);
	img_free(bitmap);
	fb_free(out);
	fclose(fb_stream);
	return 0;
}

Image *layout(Image *dest, const Image *src)
{
	if (src->width != dest->width || src->height != dest->height)
		return NULL;

	uint32_t x = 0, y = 0;
	img_for_px(x, y, 0, 0, dest->width, dest->height)
		*img_px(dest, x, y) = alpha_blend(*img_px(dest, x, y), *img_px(src, x, y));

	return dest;
}

void get_hms(double *h, double *m, double *s)
{
	struct tm ts;
	time_t tp = time(NULL);
	ts = *localtime(&tp);

	*s = (double) ts.tm_sec + get_ms() / 1000.0;
	*m = (double) ts.tm_min + *s / 60;
	*h = (double) ts.tm_hour + *m / 60;
	*h = fmod(*h, 12.0);

	return;
}

int64_t get_ms(void)
{
#if (__STDC_VERSION__ >= 201112L) 
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);

	return ts.tv_nsec / 1000000;
#else
	return 0;
#endif
}

#else
	int main(void)
	{
		fprintf(stderr, "Just for linux which support for framebuffer.\n");
		return 0;
	}
#endif