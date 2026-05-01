# SPDX-License-Identifier: MIT

AR = ar -crs
INCLUDE =-I .
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = .

DIRECTORY = $(subst $(ROOT)/,,$(shell pwd))
HDR = bmpimg.h
SRC = bmpimg.c
DEP = $(HDR) $(SRC)
OBJ = $(addsuffix .o, $(basename $(SRC)))

libbmp.a: .cc_bmpimg
	@printf "%s\t%s\n" AR $(DIRECTORY)/$@
	$(Q) $(AR) $@ $(OBJ)

.cc_bmpimg: $(DEP)
	@rm -f .cc_bmpimg
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(DIRECTORY)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c $(SRC)
	@touch .cc_bmpimg