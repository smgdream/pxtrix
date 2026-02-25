VPATH = $(ROOT)/include

INCLUDE =-I $(ROOT)/include
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = .

DIRNAME = libca
HDR = chart.h image.h
SRC = chart.c
DEP = $(HDR) $(SRC)
OBJ = $(addsuffix .o, $(basename $(SRC)))

.PHONY: compile

libca.a: .ca.tmp
	@printf "%s\t%s\n" AR $(DIRNAME)/$@
	$(Q) $(AR) $@ $(OBJ)

.ca.tmp: $(DEP)
	@rm -f $@
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(DIRNAME)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c $(SRC)
	@touch $@