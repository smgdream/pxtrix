CC = gcc
ROOT = $(shell pwd)/../
VPATH = $(ROOT)/include

INCLUDE =-I $(ROOT)/include
CFLAGS := -Wall $(INCLUDE) -O2
LDFLAGS = .

libbmp.a: bmpimg.h image.h bmpimg.c bmp2img.c
	$(CC) $(CFLAGS) -c bmpimg.c bmp2img.c
	ar -cr libbmp.a bmpimg.o bmp2img.o
