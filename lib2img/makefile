VPATH = $(ROOT)/include

INCLUDE =-I $(ROOT)/include $(INC)
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = .

DIRNAME = pic2img
HDR = image.h pixel.h
SRC = bmp2img.c qoi2img.c pgm2img.c fb2img.c ca2img.c
DEP = $(HDR) $(SRC)
OBJ = $(addsuffix .o, $(basename $(SRC)))

lib2img.a: .2img.tmp
	@printf "%s\t%s\n" AR $(DIRNAME)/$@
	$(Q) $(AR) $@ $(OBJ)

.2img.tmp: $(DEP)
	@rm -f $@
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(DIRNAME)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c $(SRC)
	@touch $@
