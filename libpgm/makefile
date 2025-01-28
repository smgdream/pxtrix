VPATH = $(ROOT)/include

INCLUDE =-I $(ROOT)/include
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = .

libpgm.a: pgm.h pgm.c pgm2img.c
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(basename $@)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c pgm.c pgm2img.c
	@printf "%s\t%s\n" AR $(basename $@)/$@
	$(Q) $(AR) $@ pgm.o pgm2img.o
