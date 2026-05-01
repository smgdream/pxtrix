# SPDX-License-Identifier: MIT

# You can modify the value of CC to change the default compiler
export CC = gcc
ifeq ($(CC), tcc)
	export AR = tcc -ar crs
else
	export AR = ar -crs
endif

# set C version
export STDC = c99
export ROOT = $(shell pwd)

AMODS_DIR = addmods
VPATH = $(ROOT)/include:util:luts:lib2img:$(AMODS_DIR)/libbmp:$(AMODS_DIR)/libqoi: \
		$(AMODS_DIR)/libpgm:$(AMODS_DIR)/libfb:$(AMODS_DIR)/libca
VERSION = $(shell cat VERSION)
# in: "version[dev_devver1-devver2]" -> process() -> out: "version[.devver1.devver2]"
export VER_NUM = $(shell cat VERSION | sed 's/dev_/./g' | sed 's/-/./g'| sed 's/dev//g')

# Default arguments
# verbose flag
V = 0
# static flag
ifeq ($(shell uname -s), Linux)
	S = 1
else
	S = 0
endif

# debug flag
DEBUG = 0

# Set verbose yes or not
ifeq ($(V), 0)
	export Q = @
	export MAKE_FLAGS = --no-print-directory
	# line delete: make * is up to date
	UP_DEL= | sed '/is up to date./d'
endif

# set debug flags
ifeq ($(DEBUG), 0)
	DEBUG_FLAGS = -D NDEBUG
	OPT_LV=-O2
else
	DEBUG_FLAGS = -D DEBUG -g2
	OPT_LV=-Og
endif

# Set the suffix of target binary file
ifeq ($(shell uname -s), Windows_NT)
	SUFF = exe
	LIB_PROC = lib_proc_win
	SO_PROC = so_lib_proc_win
else
	SUFF = run
	LIB_PROC = lib_proc_lux
	SO_PROC = so_lib_proc_lux
	FAKEROOT=fakeroot
endif

# Set args dep on compiler
ifeq ($(CC), tcc) # Is tcc
	ifeq ($(shell uname -s), Linux)
		LIBMATH = -lm
	else
		LIBMATH =
	endif
	TCC_BASE_CFLAGS =
else # Is not tcc
	LIBMATH = -lm
	ifeq ($(S), 1)
		STATIC = -static
	endif
	TCC_BASE_CFLAGS =
endif



INCLUDE = -I include -I lib2img -I $(AMODS_DIR)/libbmp -I $(AMODS_DIR)/libqoi \
		  -I $(AMODS_DIR)/libpgm -I $(AMODS_DIR)/libfb -I $(AMODS_DIR)/libca
export INC = -I $(ROOT)/pic2img -I $(ROOT)/$(AMODS_DIR)/libbmp \
			 -I $(ROOT)/$(AMODS_DIR)/libqoi -I $(ROOT)/$(AMODS_DIR)/libpgm \
			 -I $(ROOT)/$(AMODS_DIR)/libfb -I $(ROOT)/$(AMODS_DIR)/libca
export BASE_CFLAGS = -std=$(STDC) -Wall -Wextra -pedantic $(TCC_BASE_CFLAGS) -fPIC \
					 $(OPT_LV) $(DEBUG_FLAGS)
CFLAGS := $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = $(LIBMATH) -pthread


TARGET = pxtrix.$(SUFF)
TAR_DIR = build
TAR_INC = $(TAR_DIR)/include
TAR_LIB = $(TAR_DIR)/lib

OBJ_TARS = pixel.o image.o render.o util.o perf.o
LIB_TARS = lib2img libbmp libqoi libpgm libfb libca
USAGE_FILE = $(TAR_DIR)/USAGE.md

.PHONY: release test lib lib_proc_win lib_proc_lux \
		so_lib so_lib_proc_win so_lib_proc_lux \
		clean distclean outclean dist count help $(LIB_TARS)

OBJ = main.o build/lib/libpxtrix.a.*
release: lib main.o
	@printf "%s\t%s\n" LD $(TARGET)
	$(Q) $(CC) $(CFLAGS) $(STATIC) -o $(TARGET) $(OBJ) $(LDFLAGS)

lib: outclean $(LIB_TARS) $(OBJ_TARS)
	@printf "%s\t%s\n" AR libpxtrix.a.$(VER_NUM)
	$(Q) find -name "*.o" | xargs $(AR) libpxtrix.a.$(VER_NUM)

	@printf "%s\t%s\n" MKDIR "build/{lib,include/lut}"
	$(Q)- mkdir $(TAR_DIR)
	$(Q)- mkdir $(TAR_LIB)
	$(Q)- mkdir $(TAR_INC)
	$(Q)- mkdir $(TAR_INC)/luts

	@printf "%s\t%s -> %s\n" CP libpxtrix.a.$(VER_NUM) $(TAR_LIB)
	$(Q) cp libpxtrix.a.$(VER_NUM) $(TAR_LIB)

	$(Q) $(MAKE) $(MAKE_FLAGS) -C . $(LIB_PROC)

	@printf "%s\t%s -> %s\n" CP header-file $(TAR_INC)
	$(Q) cp -r include/*.h					$(TAR_INC)
	$(Q) cp lib2img/pic2img.h				$(TAR_INC)
	$(Q) cp $(AMODS_DIR)/libbmp/bmpimg.h	$(TAR_INC)
	$(Q) cp $(AMODS_DIR)/libqoi/qoimg.h		$(TAR_INC)
	$(Q) cp $(AMODS_DIR)/libpgm/pgm.h		$(TAR_INC)
	$(Q) cp $(AMODS_DIR)/libfb/fbdev.h		$(TAR_INC)
	$(Q) cp $(AMODS_DIR)/libca/chart.h		$(TAR_INC)
	
	@printf "%s\t%s -> %s\n" CP luts/\lut\*.h $(TAR_INC)/luts
	$(Q) cp -r luts/lut*.h $(TAR_INC)/luts

	@printf "%s\t%s -> %s\n" CP LICENSE $(TAR_DIR)
	$(Q) cp LICENSE $(TAR_DIR)

	@printf "%s\t%s -> %s\n" CP VERSION $(TAR_DIR)
	$(Q) cp VERSION $(TAR_DIR)

	@printf "%s\t%s\n" MKDOC $(USAGE_FILE)
	$(Q) printf "# Usage\n\n" > $(USAGE_FILE)
	$(Q) printf "## Linux\n" >> $(USAGE_FILE)
	$(Q) printf "\`\`\`sh\n" >> $(USAGE_FILE)
	$(Q) printf "%s %s %s %s %s\n" \
		"gcc" \
		"-std=$(STDC) -Wall -Wextra -pedantic" \
		"-I INCLUDE_DIR -O2 -o main.run main.c" \
		"-L LIBRARY_DIR -lpxtrix" \
		"-lm" >> $(USAGE_FILE)
	$(Q) printf "\`\`\`\n\n" >> $(USAGE_FILE)

	$(Q) printf "## Win\n" >> $(USAGE_FILE)
	$(Q) printf "\`\`\`bat\n" >> $(USAGE_FILE)
	$(Q) printf "%s %s %s %s\n" \
		"gcc" \
		"-std=$(STDC) -Wall -Wextra -pedantic" \
		"-I INCLUDE_DIR -O2 -o main.exe main.c" \
		"libpxtrix.a.$(VER_NUM)" >> $(USAGE_FILE)
	$(Q) printf "\`\`\`\n\n" >> $(USAGE_FILE)

	$(Q) printf "## example\n" >> $(USAGE_FILE)
	$(Q) printf "\`\`\`sh\n" >> $(USAGE_FILE)
	$(Q) printf "%s %s %s %s %s\n" \
		"gcc" \
		"-std=$(STDC) -Wall -Wextra -pedantic" \
		"-I build/include -O2 -o main.run main.c" \
		"-L build/lib -lpxtrix" \
		"-lm" >> $(USAGE_FILE)
	$(Q) printf "\`\`\`\n" >> $(USAGE_FILE)

lib_proc_win: # don't need to do anything
	@printf ""
	
lib_proc_lux:
	@printf "%s\t%s -> %s\n" LINK libpxtrix.a libpxtrix.a.$(VER_NUM)
	$(Q) ln -s libpxtrix.a.$(VER_NUM) $(TAR_LIB)/libpxtrix.a

so_lib: outclean $(LIB_TARS) $(OBJ_TARS)
	@printf "%s\t%s\n" LD libpxtrix.so.$(VER_NUM) 
	$(Q) find . -name "*.o" | xargs $(CC) -shared -o libpxtrix.so.$(VER_NUM) 
	
	@printf "%s\t%s\n" MKDIR "build/{lib,include/lut}"
	$(Q)- mkdir $(TAR_DIR)
	$(Q)- mkdir $(TAR_LIB)
	$(Q)- mkdir $(TAR_INC)
	$(Q)- mkdir $(TAR_INC)/luts

	@printf "%s\t%s -> %s\n" CP libpxtrix.so.$(VER_NUM) $(TAR_LIB)
	$(Q) cp libpxtrix.so.$(VER_NUM) $(TAR_LIB)

	$(Q) $(MAKE) $(MAKE_FLAGS) -C . $(SO_PROC)

	@printf "%s\t%s -> %s\n" CP header-file $(TAR_INC)
	$(Q) cp -r include/*.h					$(TAR_INC)
	$(Q) cp lib2img/pic2img.h				$(TAR_INC)
	$(Q) cp $(AMODS_DIR)/libbmp/bmpimg.h	$(TAR_INC)
	$(Q) cp $(AMODS_DIR)/libqoi/qoimg.h		$(TAR_INC)
	$(Q) cp $(AMODS_DIR)/libpgm/pgm.h		$(TAR_INC)
	$(Q) cp $(AMODS_DIR)/libfb/fbdev.h		$(TAR_INC)
	$(Q) cp $(AMODS_DIR)/libca/chart.h		$(TAR_INC)

	@printf "%s\t%s -> %s\n" CP "luts/lut*.h" $(TAR_INC)/luts
	$(Q) cp -r luts/lut*.h $(TAR_INC)/luts

	@printf "%s\t%s -> %s\n" CP LICENSE $(TAR_DIR)
	$(Q) cp LICENSE $(TAR_DIR)

	@printf "%s\t%s -> %s\n" CP VERSION $(TAR_DIR)
	$(Q) cp VERSION $(TAR_DIR)

	@printf "%s\t%s\n" MKDOC $(USAGE_FILE)
	$(Q) printf "# Usage\n\n" > $(USAGE_FILE)
	$(Q) printf "## Linux\n" >> $(USAGE_FILE)
	$(Q) printf "\`\`\`sh\n" >> $(USAGE_FILE)
	$(Q) printf "%s %s %s %s %s\n" \
		"gcc" \
		"-std=$(STDC) -Wall -Wextra -pedantic" \
		"-I include -O2 -o test.run main.c" \
		"-Wl,-rpath='relative/path/to/libpxtrix.so_s_directory'" \
		"-L lib -lpxtrix -lm" >> $(USAGE_FILE)
	$(Q) printf "\`\`\`\n\n" >> $(USAGE_FILE)

	$(Q) printf "## Win\n" >> $(USAGE_FILE)
	$(Q) printf "\`\`\`bat\n" >> $(USAGE_FILE)
	$(Q) printf "%s %s %s %s\n" \
		"gcc" \
		"-std=$(STDC) -Wall -Wextra -pedantic" \
		"-I include -O2 -o test.exe main.c" \
		"lib/libpxtrix.so.$(VER_NUM)" >> $(USAGE_FILE)
	$(Q) printf "cp lib/libpxtrix.so.$(VER_NUM) .\n"  >> $(USAGE_FILE)
	$(Q) printf "\`\`\`\n" >> $(USAGE_FILE)
	

so_lib_proc_win: # don't need to do anything
	@printf ""
	
so_lib_proc_lux:
	@printf "%s\t%s -> %s\n" LINK libpxtrix.so libpxtrix.so.$(VER_NUM)
	$(Q) ln -s libpxtrix.so.$(VER_NUM) $(TAR_LIB)/libpxtrix.so
	

main.o: main.c util.h perf.h image.h pixel.h bmpimg.h pgm.h pixel.h render.h image.h
	@printf "%s\t%s\n" CC $(shell echo $@ | sed 's/.o/.c/g')
	$(Q) $(CC) $(CFLAGS) -o $@ -c main.c

pixel.o: pixel.c pixel.h
	@printf "%s\t%s\n" CC pixel.c
	$(Q) $(CC) $(CFLAGS) -c -o $@ pixel.c

render.o: render.c render.h image.h pixel.h
	@printf "%s\t%s\n" CC $(shell echo $@ | sed 's/.o/.c/g')
	$(Q) $(CC) $(CFLAGS) -c -o $@ render.c

image.o: image.c image.h pixel.h
	@printf "%s\t%s\n" CC $(shell echo $@ | sed 's/.o/.c/g')
	$(Q) $(CC) $(CFLAGS) -c -o $@ image.c

perf.o: perf.c perf.h
	@printf "%s\t%s\n" CC util/$(shell echo $@ | sed 's/.o/.c/g')
	$(Q) $(CC) $(CFLAGS) -c -o util/$@ util/perf.c

util.o: util.c util.h
	@printf "%s\t%s\n" CC util/$(shell echo $@ | sed 's/.o/.c/g')
	$(Q) $(CC) $(CFLAGS) -c -o util/$@ util/util.c

libbmp:
	$(Q) $(MAKE) $(MAKE_FLAGS) -C $(AMODS_DIR)/libbmp .cc_bmpimg $(UP_DEL)

libqoi:
	$(Q) $(MAKE) $(MAKE_FLAGS) -C $(AMODS_DIR)/libqoi .cc_qoimg $(UP_DEL)

libpgm:
	$(Q) $(MAKE) $(MAKE_FLAGS) -C $(AMODS_DIR)/libpgm .cc_pgm $(UP_DEL)

libfb:
	$(Q) $(MAKE) $(MAKE_FLAGS) -C $(AMODS_DIR)/libfb .cc_fbdev $(UP_DEL)

libca:
	$(Q) $(MAKE) $(MAKE_FLAGS) -C $(AMODS_DIR)/libca .cc_chart $(UP_DEL)

lib2img:
	$(Q) $(MAKE) $(MAKE_FLAGS) -C lib2img .cc_pic2img $(UP_DEL)

test:
	$(Q) echo Testing start
	$(Q) echo
	$(Q) rm -f tests/*.out tests/*.exe tests/tmp/*.bmp tests/tmp/*.qoi tests/tmp/*.pgm
	$(Q) $(MAKE) $(MAKE_FLAGS) -C tests

clean: outclean
	@echo CLEAN
	$(Q) find . -name "*.o" | xargs rm -f
	$(Q) find . -name "*.a"	| grep -v "$(TAR_DIR)/" | xargs rm -f
	$(Q) find . -name ".cc_*" | xargs rm -f
	

distclean: clean
	@echo DISTCLEAN
	$(Q) rm -f *.bmp
	$(Q) rm -f *.qoi
	$(Q) rm -f *.pgm
	$(Q) rm -f *.png
	$(Q) rm -f tests/tmp/*.qoi
	$(Q) rm -f tests/tmp/*.pgm

outclean: 
	@printf "CLEAN\toutput files\n"
	$(Q) find . -name "*.so.*"	| grep -v "$(TAR_DIR)/" | xargs rm -f
	$(Q) find . -name "*.a.*"	| grep -v "$(TAR_DIR)/" | xargs rm -f
	$(Q) find . -name "*.run*"	| grep -v "$(TAR_DIR)/" | xargs rm -f
	$(Q) find . -name "*.out*"	| grep -v "$(TAR_DIR)/" | xargs rm -f
	$(Q) find . -name "*.exe*"	| grep -v "$(TAR_DIR)/" | xargs rm -f
	$(Q) rm -fr build

dist:
	@echo $(VERSION)
	@printf "%s\t%s\n" CP "./ -> ../pxtrix-$(VERSION)"
	$(Q) cp -r . ../pxtrix-$(VERSION)
	$(Q) $(MAKE) $(MAKE_FLAG) -C ../pxtrix-$(VERSION) distclean
	$(Q) rm -f ../pxtrix-$(VERSION)/test.c
	$(Q) rm -f ../pxtrix-$(VERSION)/*.bmp
	$(Q) rm -f ../pxtrix-$(VERSION)/*.qoi
	$(Q) rm -f ../pxtrix-$(VERSION)/*.pgm
	$(Q) rm -f ../pxtrix-$(VERSION)/*.png
	$(Q) rm -f ../pxtrix-$(VERSION)/*.run
	$(Q) rm -f ../pxtrix-$(VERSION)/*.exe
	$(Q) rm -fr ../pxtrix-$(VERSION)/.vscode
	$(Q) rm -fr ../pxtrix-$(VERSION)/test
	$(Q) rm -fr ../pxtrix-$(VERSION)/tools
	$(Q) rm -fr ../pxtrix-$(VERSION)/tests/tmp/*
	$(Q) rm -fr ../pxtrix-$(VERSION)/tests/pictures/*
	@printf "%s\t%s\n" TAR "../pxtrix-$(VERSION).tar.xz"
	$(Q) $(FAKEROOT) tar -caf ../pxtrix-$(VERSION).tar.xz \
		--exclude=../pxtrix-$(VERSION)/.vscode ../pxtrix-$(VERSION)
	@printf "%s\t%s\n" RM "../pxtrix-$(VERSION)"
	$(Q) rm -rf ../pxtrix-$(VERSION)

count:
	@cloc --exclude-ext=md,json --exclude-dir=build,docs,test,tests,tools .

help:
	@awk '/Enviroment requirement/, /Count lines of code/' README.md
