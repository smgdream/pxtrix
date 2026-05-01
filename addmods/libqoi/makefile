# SPDX-License-Identifier: MIT

AR = ar -crs
INCLUDE =-I .
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS =

DIRECTORY = $(subst $(ROOT)/,,$(shell pwd))
HDR = qoimg.h qoicodec.h bswap.h
SRC = qoimg.c qoicodec.c
DEP = $(HDR) $(SRC)
OBJ = $(addsuffix .o, $(basename $(SRC)))

libqoi.a: .cc_qoimg
	@printf "%s\t%s\n" AR $(DIRECTORY)/$@
	$(Q) $(AR) $@ $(OBJ)

.cc_qoimg: $(DEP)
	@rm -f .cc_qoimg
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(DIRECTORY)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c $(SRC)
	@touch .cc_qoimg