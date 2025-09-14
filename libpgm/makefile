VPATH = $(ROOT)/include

INCLUDE =-I $(ROOT)/include
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = .

DIRNAME = libpgm
HDR = pgm.h image.h 
SRC = pgm.c
DEP = $(HDR) $(SRC)
OBJ = $(addsuffix .o, $(basename $(SRC)))


libpgm.a: .pgm.tmp
	@printf "%s\t%s\n" AR $(DIRNAME)/$@
	$(Q) $(AR) $@ $(OBJ)

.pgm.tmp: $(DEP)
	@rm -f $@
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(DIRNAME)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c $(SRC)
	@touch $@