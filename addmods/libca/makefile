# SPDX-License-Identifier: MIT

AR = ar -crs
INCLUDE =-I .
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = .

DIRECTORY = $(subst $(ROOT)/,,$(shell pwd))
HDR = chart.h
SRC = chart.c
DEP = $(HDR) $(SRC)
OBJ = $(addsuffix .o, $(basename $(SRC)))

libca.a: .cc_chart
	@printf "%s\t%s\n" AR $(DIRECTORY)/$@
	$(Q) $(AR) $@ $(OBJ)

.cc_chart: $(DEP)
	@rm -f .cc_chart
	@for c_file in $(filter %.c, $^);\
	do\
		printf "%s\t%s\n" CC $(DIRECTORY)/$$c_file;\
	done
	$(Q) $(CC) $(CFLAGS) -c $(SRC)
	@touch .cc_chart