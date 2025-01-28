# You can modify the value of CC to change the compiler
export CC = gcc
export AR = ar -crs

export STDC = c99
export ROOT = $(shell pwd)

VPATH = $(ROOT)/include:libbmp:libqoi:libpgm:tools
VERSION = $(shell cat VERSION)

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
else
	SUFF = run
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



INCLUDE = -I ./include -I ./libbmp -I ./libqoi -I ./libpgm
export BASE_CFLAGS = -std=$(STDC) -Wall -Wextra $(TCC_BASE_CFLAGS)
CFLAGS := -O2 $(BASE_CFLAGS) -pedantic $(INCLUDE)
LDFLAGS = -pthread $(LIBMATH)

OBJ = main.o render.o perf.o image.o libbmp/libbmp.a libqoi/libqoi.a libpgm/libpgm.a
TARGET = pxtrix.$(SUFF)

.PHONY: lib clean cleanall dist count help libbmp libqoi libpgm

release: main.o render.o image.o perf.o libbmp libqoi libpgm
	@printf "%s\t%s\n" LD $(TARGET)
	$(Q) $(CC) $(CFLAGS) $(STATIC) -o $(TARGET) $(OBJ) $(LDFLAGS)
lib: libbmp libqoi libpgm image.o render.o include/*.h luts/*.lut
	$(Q) printf "%s\t%s\n" AR libimg.a
	$(Q) $(AR) libimg.a image.o
	$(Q) printf "%s\t%s\n" AR librender.a
	$(Q) $(AR) librender.a render.o

	$(Q) printf "%s\n" MKDIR
	$(Q) mkdir build
	$(Q) mkdir build/lib
	$(Q) mkdir build/include
	$(Q) mkdir build/include/luts

	$(Q) printf "%s\t%s -> %s\n" CP archive-file build/lib
	$(Q) cp libbmp/libbmp.a build/lib
	$(Q) cp libqoi/libqoi.a build/lib
	$(Q) cp libpgm/libpgm.a build/lib
	$(Q) cp libimg.a build/lib
	$(Q) cp librender.a build/lib

	$(Q) printf "%s\t%s -> %s\n" CP header-file build/include
	$(Q) cp -r include/*.h build/include
	$(Q) cp libbmp/bmpimg.h build/include/
	$(Q) cp libqoi/qoimg.h build/include/
	$(Q) cp libpgm/pgm.h build/include/
	$(Q) printf "%s\t%s -> %s\n" CP luts/\*.lut build/include/luts
	$(Q) cp -r luts/*.lut build/include/luts
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
	$(Q) $(CC) $(CFLAGS) -o $@ -c tools/perf.c

libbmp:
	$(Q) $(MAKE) $(MAKE_FLAG) -C libbmp

libqoi:
	$(Q) $(MAKE) $(MAKE_FLAG) -C libqoi

libpgm:
	$(Q) $(MAKE) $(MAKE_FLAG) -C libpgm

clean:
	@printf "%s\t%s\n" RM "*.o *.a"
	$(Q) find . -name "*.o" | xargs rm -f
	$(Q) find . -name "*.a" | xargs rm -f

cleanall: clean
	@echo CLEAN
	$(Q) rm -f *.bmp
	$(Q) rm -f *.qoi
	$(Q) rm -f *.pgm
	$(Q) rm -f *.png
	$(Q) rm -f pxtrix.run
	$(Q) rm -f pxtrix.exe
	$(Q) rm -fr build

dist:
	@echo $(VERSION)
	@printf "%s\t%s\n" CP "./ -> ../pxtrix-$(VERSION)"
	$(Q) cp -r . ../pxtrix-$(VERSION)
	$(Q) $(MAKE) $(MAKE_FLAG) -C ../pxtrix-$(VERSION) cleanall
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
	@cloc --exclude-ext=md .

help:
	@cat README.md | tail -n 33 | head -n 30
