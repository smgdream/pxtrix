# SPDX-License-Identifier: MIT

INCLUDE		= ../build/include
LIB_DIR		= ../build/lib
CFLAGS		= -Wall -Wextra -I $(INCLUDE) -O

ifeq ($(shell uname -s), Windows_NT)
	PXTRIX_LIBS	= $(LIB_DIR)/libpxtrix.a.*
	SUFF = exe
else
	PXTRIX_LIBS	= -L $(LIB_DIR) -lpxtrix
	LIBMATH=-lm
	SUFF = out
endif

LDFLAGS		= $(PXTRIX_LIBS) $(LIBMATH)
IMGSRC		= pictures
OUT_DIR		= tmp

# /dev/stdout
OUT_TAR_FILE=/dev/null
DELAY=1.5s

test: bmpimg.$(SUFF) pixel.$(SUFF) image0.$(SUFF) qoimg.$(SUFF) pgm.$(SUFF) \
	  fbdev.$(SUFF) chart.$(SUFF) render.$(SUFF) luts.$(SUFF) util.$(SUFF) image1.$(SUFF)
	$(Q) echo
	$(Q) echo Testing end

bmpimg.$(SUFF): $(shell find -name "*_bmpimg.c")
	$(Q) $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(Q) ./$@ $(IMGSRC)/TM8.bmp $(OUT_DIR)/$@.bmp >> $(OUT_TAR_FILE)
	$(Q) printf "%-28s%cK\n" $^ $$?

pixel.$(SUFF): $(shell find -name "*_pixel.c")
	$(Q) $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(Q) ./$@ >> $(OUT_TAR_FILE)
	$(Q) printf "%-28s%cK\n" $^ $$?

image0.$(SUFF): $(shell find -name "*_image0.c")
	$(Q) $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(Q) ./$@ $(IMGSRC)/TM8.bmp $(OUT_DIR)/$@.bmp >> $(OUT_TAR_FILE)
	$(Q) printf "%-28s%cK\n" $^ $$?

qoimg.$(SUFF): $(shell find -name "*_qoimg.c")
	$(Q) $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(Q) for qoi_file in \
		$$(find $(IMGSRC)/qoi_test -name "*.qoi" | sed -e 's/$(IMGSRC)\/qoi_test\///g');\
	do\
		./$@ $(IMGSRC)/qoi_test/$${qoi_file} $(OUT_DIR)/$@.$${qoi_file} \
													>> $(OUT_TAR_FILE);\
	done
	$(Q) printf "%-28s%cK\n" $^ $$?

pgm.$(SUFF): $(shell find -name "*_pgm.c")
	$(Q) $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(Q) ./$@ $(IMGSRC)/cat.qoi $(IMGSRC)/DR.fake16bit.pgm \
		$(OUT_DIR)/$@.cat.pgm $(OUT_DIR)/$@.DR.qoi 709 >> $(OUT_TAR_FILE)
	$(Q) ./$@ "$(IMGSRC)/IBM System 360.qoi" "$(IMGSRC)/ritchie-thompson.pgm" \
		"$(OUT_DIR)/$@.IBM System 360.pgm" "$(OUT_DIR)/$@.ritchie-thompson.qoi" hsv \
			 >> $(OUT_TAR_FILE)
	$(Q) printf "%-28s%cK\n" $^ $$?

fbdev.$(SUFF): $(shell find -name "*_fbdev.c")
	$(Q) $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(Q) ./$@ ss $(OUT_DIR)/$@.screenshot.qoi >> $(OUT_TAR_FILE)
	$(Q) ./$@ qv $(IMGSRC)/hiro.qoi			>> $(OUT_TAR_FILE)
	$(Q) sleep $(DELAY)
	$(Q) ./$@ qv $(IMGSRC)/miku_coffee.qoi	>> $(OUT_TAR_FILE)
	$(Q) sleep $(DELAY)
	$(Q) ./$@ qv $(IMGSRC)/Lmashu_Rolea.qoi	>> $(OUT_TAR_FILE)
	$(Q) sleep $(DELAY)
	$(Q) ./$@ qv $(IMGSRC)/kiki2026.qoi		>> $(OUT_TAR_FILE)
	$(Q) sleep $(DELAY)
	$(Q) ./$@ qv $(IMGSRC)/touhou.qoi		>> $(OUT_TAR_FILE)
	$(Q) sleep $(DELAY)
	$(Q) printf "%-28s%cK\n" $^ $$?

chart.$(SUFF): $(shell find -name "*_chart.c")
	$(Q) $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(Q) ./$@ $(IMGSRC)/soup.qoi | less
	$(Q) ./$@ $(IMGSRC)/minecraft0.qoi 240 1.1 | less
	$(Q) ./$@ $(IMGSRC)/Vega.qoi 225 | less
	$(Q) printf "%-28s%cK\n" $^ $$?

render.$(SUFF): $(shell find -name "*_render.c")
	$(Q) $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(Q) ./$@ mode0 $(IMGSRC)/REAPER.qoi $(IMGSRC)/Miku_v6_r.qoi $(OUT_DIR)/$@.reaper_miku.qoi
	$(Q) ./$@ mode1 $(OUT_DIR)/$@.beizer_curve.qoi
	$(Q) ./$@ mode2 $(IMGSRC)/jiangmen.qoi $(OUT_DIR)/$@.jiangmen.qoi
	$(Q) printf "%-28s%cK\n" $^ $$?
	
luts.$(SUFF): $(shell find -name "*_luts.c")
	$(Q) $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(Q) ./$@ $(OUT_DIR)/luts.qoi
	$(Q) printf "%-28s%cK\n" $^ $$?

util.$(SUFF): $(shell find -name "*_util.c")
	$(Q) $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(Q) ./$@ $(OUT_DIR)/$@.qoi >> $(OUT_TAR_FILE)
	$(Q) printf "%-28s%cK\n" $^ $$?

image1.$(SUFF): $(shell find -name "*_image1.c")
	$(Q) $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(Q) ./$@ $(IMGSRC)/Bocchi.qoi $(OUT_DIR)/$@.Bocchi.qoi
	$(Q) printf "%-28s%cK\n" $^ $$?