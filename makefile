# You can modify the value of CC to change the compiler
export CC = gcc
export AR = ar -crs

export STDC = c99
export ROOT = $(shell pwd)

VPATH = $(ROOT)/include:lib2img:tools:libbmp:libqoi:libpgm:libfb:libca
VERSION = $(shell cat VERSION)
# Version for computer e.g. version[.dev_ver1.dev_ver2]
export VER_NUM = $(shell cat VERSION | sed 's/dev_/./g' | sed 's/-/./g'| sed 's/dev//g')

# Default arguments
# verbose flag
V = 0
# static flag
S = 1

# Set verbose yes or not
ifeq ($(V), 1)
	Q_FLAG = 0
else
	Q_FLAG = 1
endif
ifeq ($(Q_FLAG), 1)
	export Q = @
	export MAKE_FLAG = --no-print-directory
else
	export Q =
	export MAKE_FLAG =
endif

# Set the suffix of target binary file
ifeq ($(shell uname -s), Windows_NT)
	SUFF = exe
	SO_PROC = so_proc_win
else
	SUFF = run
	SO_PROC = so_proc_lux
	PERF_LIB = -lrt
endif

# Set args dep on compile
ifeq ($(CC), tcc)
# Is tcc
	ifeq ($(shell uname -s), Linux)
		LIBMATH = -lm
	else
		LIBMATH =
	endif
	export STATIC = 
	TCC_BASE_CFLAGS =
else
# Is not tcc
	LIBMATH = -lm
	ifeq ($(S), 1)
		export STATIC = -static
	endif
	TCC_BASE_CFLAGS =
endif



INCLUDE = -I ./include -I ./lib2img -I ./libbmp -I ./libqoi -I ./libpgm -I ./libfb -I ./libca
export INC = -I $(ROOT)/pic2img -I $(ROOT)/libbmp -I $(ROOT)/libqoi -I $(ROOT)/libpgm -I $(ROOT)/libfb -I $(ROOT)/libca
export BASE_CFLAGS = -std=$(STDC) -Wall -Wextra -pedantic $(TCC_BASE_CFLAGS)  -fPIC
CFLAGS := -O2 $(BASE_CFLAGS) $(INCLUDE)
LDFLAGS = $(LIBMATH) -pthread

OBJ = main.o render.o perf.o image.o lib2img/lib2img.a libbmp/libbmp.a libqoi/libqoi.a libpgm/libpgm.a libfb/libfb.a libca/libca.a
TARGET = pxtrix.$(SUFF)
TAR_DIR = build
TAR_INC = $(TAR_DIR)/include
TAR_LIB = $(TAR_DIR)/lib

.PHONY: lib clean cleanall dist count help lib2img libbmp libqoi libpgm libfb libca

release: main.o render.o image.o perf.o lib2img libbmp libqoi libpgm libfb libca
	@printf "%s\t%s\n" LD $(TARGET)
	$(Q) $(CC) $(CFLAGS) $(STATIC) -o $(TARGET) $(OBJ) $(LDFLAGS)
lib: lib2img libbmp libqoi libpgm libfb libca image.o render.o
	@printf "%s\t%s\n" AR libimg.a
	$(Q) $(AR) libimg.a image.o
	@printf "%s\t%s\n" AR librender.a
	$(Q) $(AR) libren.a render.o

	@printf "%s\n" MKDIR
	$(Q)- mkdir $(TAR_DIR)
	$(Q)- mkdir $(TAR_LIB)
	$(Q)- mkdir $(TAR_INC)
	$(Q)- mkdir $(TAR_INC)/luts

	@printf "%s\t%s -> %s\n" CP archive-files $(TAR_LIB)
	$(Q) find . -name "*.a" | grep -v "$(TAR_DIR)/" | xargs -I src cp src $(TAR_LIB)

	@printf "%s\t%s -> %s\n" CP header-file $(TAR_INC)
	$(Q) cp -r include/*.h		$(TAR_INC)
	$(Q) cp lib2img/pic2img.h	$(TAR_INC)
	$(Q) cp libbmp/bmpimg.h		$(TAR_INC)
	$(Q) cp libqoi/qoimg.h		$(TAR_INC)
	$(Q) cp libpgm/pgm.h		$(TAR_INC)
	$(Q) cp libfb/fbdev.h		$(TAR_INC)
	$(Q) cp libca/chart.h		$(TAR_INC)
	
	@printf "%s\t%s -> %s\n" CP luts/\lut_\*.h $(TAR_INC)/luts
	$(Q) cp -r luts/lut_*.h $(TAR_INC)/luts

	@printf "%s\t%s -> %s\n" CP tools/perf.c $(TAR_DIR)
	$(Q) cp tools/perf.c $(TAR_DIR)

	@printf "%s\t%s -> %s\n" CP LICENSE $(TAR_DIR)
	$(Q) cp LICENSE $(TAR_DIR)

	@printf "%s\t%s -> %s\n" CP VERSION $(TAR_DIR)
	$(Q) cp VERSION $(TAR_DIR)

	@printf "%s\t%s\n" MKDOC $(TAR_DIR)/USAGE
	$(Q) printf "Usage:\n" > $(TAR_DIR)/USAGE
	$(Q) printf "%s %s %s %s %s\n" "gcc" \
		"-std=$(STDC) -Wall -Wextra -pedantic" \
		"-I ./include -O2 -o test.run main.c" \
		"ARCHIVE-FILES" \
		"-lm -pthread" >> $(TAR_DIR)/USAGE

so_lib: image.o render.o lib2img_c libbmp_c libqoi_c libpgm_c libfb_c libca_c
	@printf "%s\t%s\n" LD libpxtrix.so.$(VER_NUM) 
	$(Q) find . -name "*.o" | xargs $(CC) -shared -o libpxtrix.so.$(VER_NUM) 
	
	@printf "%s\n" MKDIR
	$(Q)- mkdir $(TAR_DIR)
	$(Q)- mkdir $(TAR_LIB)
	$(Q)- mkdir $(TAR_INC)
	$(Q)- mkdir $(TAR_INC)/luts

	@printf "%s\t%s -> %s\n" CP libpxtrix.so.$(VER_NUM) $(TAR_LIB)
	$(Q) cp libpxtrix.so.$(VER_NUM) $(TAR_LIB)

	$(Q) $(MAKE) $(MAKE_FLAG) -C . $(SO_PROC)

	@printf "%s\t%s -> %s\n" CP header-file $(TAR_INC)
	$(Q) cp -r include/*.h		$(TAR_INC)
	$(Q) cp lib2img/pic2img.h	$(TAR_INC)
	$(Q) cp libbmp/bmpimg.h		$(TAR_INC)
	$(Q) cp libqoi/qoimg.h		$(TAR_INC)
	$(Q) cp libpgm/pgm.h		$(TAR_INC)
	$(Q) cp libfb/fbdev.h		$(TAR_INC)
	$(Q) cp libca/chart.h		$(TAR_INC)

	@printf "%s\t%s -> %s\n" CP luts/\lut_\*.h $(TAR_INC)/luts
	$(Q) cp -r luts/lut_*.h $(TAR_INC)/luts

	@printf "%s\t%s -> %s\n" CP tools/perf.c $(TAR_DIR)
	$(Q) cp tools/perf.c $(TAR_DIR)

	@printf "%s\t%s -> %s\n" CP LICENSE $(TAR_DIR)
	$(Q) cp LICENSE $(TAR_DIR)

	@printf "%s\t%s -> %s\n" CP VERSION $(TAR_DIR)
	$(Q) cp VERSION $(TAR_DIR)

	@printf "%s\t%s\n" MKDOC $(TAR_DIR)/USAGE
	$(Q) printf "Usage:\n\n" > $(TAR_DIR)/USAGE
	$(Q) printf "Linux:\n%s %s %s %s %s\n\n" "gcc" \
		"-std=$(STDC) -Wall -Wextra -pedantic" \
		"-I ./include -L ./lib -Wl,-rpath='./lib/' -O2" \
		"-o test.run main.c " \
		"-lpxtrix -lm -pthread" >> $(TAR_DIR)/USAGE
	$(Q) printf "Win:\n%s %s %s %s\n" "gcc" \
		"-std=$(STDC) -Wall -Wextra -pedantic" \
		"-I ./include -O2 -o test.exe main.c lib/libpxtrix.so.$(VER_NUM)" \
		"-lm -pthread" >> $(TAR_DIR)/USAGE
	$(Q) printf "cp lib/libpxtrix.so.$(VER_NUM) .\n"  >> $(TAR_DIR)/USAGE
	

so_proc_win:
	@printf ""
	
so_proc_lux:
	@printf "%s\t%s -> %s\n" LINK libpxtrix.so libpxtrix.so.$(VER_NUM)
	$(Q) ln -s libpxtrix.so.$(VER_NUM)	$(TAR_LIB)/libpxtrix.so
	

main.o: main.c util.h perf.h image.h pixel.h bmpimg.h pgm.h pixel.h render.h image.h
	@printf "%s\t%s\n" CC $(shell echo $@ | sed 's/.o/.c/g')
	$(Q) $(CC) $(CFLAGS) -o $@ -c main.c

render.o: render.c render.h image.h pixel.h
	@printf "%s\t%s\n" CC $(shell echo $@ | sed 's/.o/.c/g')
	$(Q) $(CC) $(CFLAGS) -o $@ -c render.c

image.o: image.c image.h pixel.h
	@printf "%s\t%s\n" CC $(shell echo $@ | sed 's/.o/.c/g')
	$(Q) $(CC) $(CFLAGS) -o $@ -c image.c

perf.o: perf.c perf.h
	@printf "%s\t%s\n" CC tools/$(shell echo $@ | sed 's/.o/.c/g')
	$(Q) $(CC) $(CFLAGS) -o $@ -c tools/perf.c $(PERF_LIB)

libbmp:
	$(Q) $(MAKE) $(MAKE_FLAG) -C libbmp
libbmp_c:
	$(Q) $(MAKE) $(MAKE_FLAG) -C libbmp .bmp.tmp

libqoi:
	$(Q) $(MAKE) $(MAKE_FLAG) -C libqoi
libqoi_c:
	$(Q) $(MAKE) $(MAKE_FLAG) -C libqoi .qoi.tmp

libpgm:
	$(Q) $(MAKE) $(MAKE_FLAG) -C libpgm
libpgm_c:
	$(Q) $(MAKE) $(MAKE_FLAG) -C libpgm .pgm.tmp

libfb:
	$(Q) $(MAKE) $(MAKE_FLAG) -C libfb
libfb_c:
	$(Q) $(MAKE) $(MAKE_FLAG) -C libfb .fb.tmp

libca:
	$(Q) $(MAKE) $(MAKE_FLAG) -C libca
libca_c:
	$(Q) $(MAKE) $(MAKE_FLAG) -C libca .ca.tmp

lib2img:
	$(Q) $(MAKE) $(MAKE_FLAG) -C lib2img
lib2img_c:
	$(Q) $(MAKE) $(MAKE_FLAG) -C lib2img .2img.tmp

clean:
	@printf "%s\t%s\n" RM "*.o *.a *.so"
	$(Q) find . -name "*.o" | xargs rm -f
	$(Q) find . -name "*.a"		| grep -v "$(TAR_DIR)/" | xargs rm -f
	$(Q) find . -name "*.so.$(VER_NUM)"	| grep -v "$(TAR_DIR)/" | xargs rm -f
	$(Q) find . -name ".*.tmp"	| xargs rm -f

distclean: clean
	@echo CLEAN
	$(Q) find . -name "*.so.*"	| grep -v "$(TAR_DIR)/" | xargs rm -f
	$(Q) rm -f *.bmp
	$(Q) rm -f *.qoi
	$(Q) rm -f *.pgm
	$(Q) rm -f *.png
	$(Q) rm -fr build
	$(Q) rm -f pxtrix.run
	$(Q) rm -f pxtrix.exe
	
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
	@printf "%s\t%s\n" TAR "../pxtrix-$(VERSION).tar.xz"
	$(Q) tar -caf ../pxtrix-$(VERSION).tar.xz --exclude=../pxtrix-$(VERSION)/.vscode ../pxtrix-$(VERSION)
	@printf "%s\t%s\n" RM "../pxtrix-$(VERSION)"
	$(Q) rm -rf ../pxtrix-$(VERSION)

count:
	@cloc --exclude-ext=md,json --exclude-dir=luts,build,doc .

help:
	@awk '/# C version: C99/, /make count/' README.md
