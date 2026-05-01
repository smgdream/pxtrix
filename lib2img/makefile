# SPDX-License-Identifier: MIT

VPATH = $(ROOT)/include

INCLUDE =-I $(ROOT)/include $(INC)
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = .

DIRECTORY = $(subst $(ROOT)/,,$(shell pwd))
HDR = image.h pixel.h
SRC = bmp2img.c qoi2img.c pgm2img.c fb2img.c ca2img.c
DEP = $(HDR) $(SRC)
OBJ = $(addsuffix .o, $(basename $(SRC)))

lib2img.a: .cc_pic2img
	@printf "%s\t%s\n" AR $(DIRECTORY)/$@
	$(Q) $(AR) $@ $(OBJ)

.cc_pic2img: $(DEP)
	@rm -f .cc_pic2img
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(DIRECTORY)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c $(SRC)
	@touch .cc_pic2img
