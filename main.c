#include <stdio.h>
#include <math.h>
#include "image.h"
#include "bmpimg.h"
#include "pic2img.h"
#include "tmath.h"

int main(void)
{
	Image *img = img_new(1080, 1080, 32, sRGB);
	Bmpimg *bmp = bmp_new(BMP_EMPTY);
	
	uint32_t img_x = 0, img_y = 0;
	img_for_px(img_x, img_y, 0, 0, img->width, img->height) {
		/* must cast img_x and img_y to double  */
		double x = (double) img_x - img->width / 2;
		double y = (double) img_y - img->height / 2;
		double r = polar_r(x, y);
		double t = polar_t(x, y);
		if (r <= img->width / 2) {
			Px_hsva hsva = { t * 360 / TAU, 2 * r / img->width, 1.0, 1.0 };
			*img_px(img, img_x, img_y) = hsva2rgba(hsva);
		} else
			*img_px(img, img_x, img_y) = rgba_hex(0x000000ff);
	}

	img2bmp(img, bmp);
	bmp_write(bmp, "out.bmp");

	img_free(img);
	bmp_free(bmp);
	return 0;
}
