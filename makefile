CC = gcc
ROOT = $(shell pwd)
VPATH = $(ROOT)/include:libbmp:libqoi:tools

INCLUDE =-I $(ROOT)/include -I ./libbmp -I ./libqoi
CFLAGS := -Wall $(INCLUDE) -O2
LDFLAGS = -lm

.PHONY: clean cleanall

release: main.o render.o image.o perf.o libbmp.a libqoi.a
	$(CC) $(CFLAGS) -static -o pxtrix.run main.o render.o perf.o image.o libbmp/libbmp.a libqoi/libqoi.a $(LDFLAGS)

main.o: image.h qoimg.h main.c
	$(CC) $(CFLAGS) -c main.c

render.o: render.h image.h render.c
	$(CC) $(CFLAGS) -c render.c

image.o: image.h pixel.h image.c
	$(CC) $(CFLAGS) -c image.c

perf.o: perf.c
	$(CC) $(CFLAGS) -c tools/perf.c

libbmp.a:
	make -C libbmp

libqoi.a:
	make -C libqoi

clean:
	find . -name "*.o" | xargs rm -fv
	find . -name "*.a" | xargs rm -fv

cleanall:
	find . -name "*.o" | xargs rm -fv
	find . -name "*.a" | xargs rm -fv
	rm pxtrix.run
