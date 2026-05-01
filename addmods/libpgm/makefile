# SPDX-License-Identifier: MIT

AR = ar -crs
INCLUDE =-I .
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = .

DIRECTORY = $(subst $(ROOT)/,,$(shell pwd))
HDR = pgm.h
SRC = pgm.c pgmcodec.c
DEP = $(HDR) $(SRC)
OBJ = $(addsuffix .o, $(basename $(SRC)))

libpgm.a: .cc_pgm
	@printf "%s\t%s\n" AR $(DIRECTORY)/$@
	$(Q) $(AR) $@ $(OBJ)

.cc_pgm: $(DEP)
	@rm -f .cc_pgm
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(DIRECTORY)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c $(SRC)
	@touch .cc_pgm