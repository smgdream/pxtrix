VPATH = $(ROOT)/include

INCLUDE =-I $(ROOT)/include
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = .

libqoi.a: qoimg.h bswap.h image.h qoimg.c qoicodec.c qoi2img.c
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(basename $@)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c qoimg.c qoicodec.c qoi2img.c
	@printf "%s\t%s\n" AR $(basename $@)/$@
	$(Q) $(AR) $@ qoimg.o qoicodec.o qoi2img.o
