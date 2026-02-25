#include <stdio.h>
#include "pixel.h"
#include "image.h"
#include "bmpimg.h"
#include "pic2img.h"

#define print_rgba(pix_ptr) printf("rgba: %hhu %hhu %hhu %hhu", (pix_ptr)->r ,(pix_ptr)->g ,(pix_ptr)->b ,(pix_ptr)->a)
static inline bool px_eq(Px_rgba a, Px_rgba b)
{
	return *(uint32_t *)&a == *(uint32_t *)&b;
}

int main(int argc, char *argv[])
{
	Bmpimg *bmp_src = bmp_read(argv[1]);
	Bmpimg *bmp_dest = bmp_new(BMP_EMPTY);
	Image *img_src = img_new(IMG_EMPTY);

	bmp2img(bmp_src, img_src);
	Image *img_dest = img_new(img_src->width, img_src->height, 32, sRGB);

	uint16_t x = 0, y = 0;
	size_t diff_cnt = 0;
	img_for_px(x, y, 0, 0, img_src->width, img_src->height) {
		Px_hsva hsva = rgba2hsva(*img_px(img_src, x, y));
		*img_px(img_dest, x, y) = hsva2rgba(hsva);
		if (!px_eq(*img_px(img_src, x, y), *img_px(img_dest, x, y))) {
			//print_rgba(img_px(img_src, x, y));
			//printf(" | ");
			//printf("%.4f %.4f %.4f %.4f",
			//	hsva.h, hsva.s, hsva.v, hsva.a);
			//printf(" | ");
			//print_rgba(img_px(img_dest, x, y));
			//printf("\n");
			++diff_cnt;
		}
	}
	printf("px diff rate: %.4f%%\n", (double) diff_cnt / (img_dest->width * img_dest->height) * 100);

	img2bmp(img_dest, bmp_dest);
	bmp_write(bmp_dest, "out.bmp");

	img_free(img_src);
	img_free(img_dest);
	bmp_free(bmp_src);
	bmp_free(bmp_dest);
	return 0;
}
/*
static inline bool px_eq(Px_rgba a, Px_rgba b)
{
	return *(uint32_t *)&a == *(uint32_t *)&b;
}

int main(void)
{
    size_t diff_cnt = 0;

    for (uint32_t hex = 0; hex <= 0xffffff; ++hex) {
        Px_rgba px0 = rgba_hex(hex << 8);
        Px_rgba px1 = hsva2rgba(rgba2hsva(px0));
        if (!px_eq(px1, px0))
            ++diff_cnt;
    }
    printf("%zu pixel difference rate: %0.4f%%\n", diff_cnt,
            (double) diff_cnt / 0x01'00'00'00 * 100);
    
    return 0;
}*/