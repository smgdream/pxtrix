VPATH = $(ROOT)/include

INCLUDE =-I $(ROOT)/include
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = .

libbmp.a: bmpimg.h image.h bmpimg.c bmp2img.c
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(basename $@)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c bmpimg.c bmp2img.c
	@printf "%s\t%s\n" AR $(basename $@)/$@
	$(Q) $(AR) $@ bmpimg.o bmp2img.o
