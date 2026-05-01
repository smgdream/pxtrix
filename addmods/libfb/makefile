# SPDX-License-Identifier: MIT

AR = ar -crs
INCLUDE =-I .
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = .

DIRECTORY = $(subst $(ROOT)/,,$(shell pwd))
HDR = fbdev.h
SRC = fbdev.c fb_dev_info.c
DEP = $(HDR) $(SRC)
OBJ = $(addsuffix .o, $(basename $(SRC)))

libfb.a: .cc_fbdev
	@printf "%s\t%s\n" AR $(DIRECTORY)/$@
	$(Q) $(AR) $@ $(OBJ)

.cc_fbdev: $(DEP)
	@rm -f .cc_fbdev
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(DIRECTORY)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c $(SRC)
	@touch .cc_fbdev