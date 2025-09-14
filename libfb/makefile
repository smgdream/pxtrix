VPATH = $(ROOT)/include

INCLUDE =-I $(ROOT)/include
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = .

DIRNAME = libfb
HDR = fbdev.h image.h 
SRC = fbdev.c fb_dev_info.c
DEP = $(HDR) $(SRC)
OBJ = $(addsuffix .o, $(basename $(SRC)))


libfb.a: .fb.tmp
	@printf "%s\t%s\n" AR $(DIRNAME)/$@
	$(Q) $(AR) $@ $(OBJ)

.fb.tmp: $(DEP)
	@rm -f $@
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(DIRNAME)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c $(SRC)
	@touch $@