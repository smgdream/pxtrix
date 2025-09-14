VPATH = $(ROOT)/include

INCLUDE =-I $(ROOT)/include
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = .

DIRNAME = libbmp
HDR = bmpimg.h image.h 
SRC = bmpimg.c
DEP = $(HDR) $(SRC)
OBJ = $(addsuffix .o, $(basename $(SRC)))


libbmp.a: .bmp.tmp
	@printf "%s\t%s\n" AR $(DIRNAME)/$@
	$(Q) $(AR) $@ $(OBJ)

.bmp.tmp: $(DEP)
	@rm -f $@
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(DIRNAME)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c $(SRC)
	@touch $@
