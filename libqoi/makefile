CC = gcc
ROOT = $(shell pwd)/../
VPATH = $(ROOT)/include

INCLUDE =-I $(ROOT)/include
CFLAGS := -Wall $(INCLUDE) -O2
LDFLAGS = .


libqoi.a: qoimg.h bswap.h image.h qoimg.c qoicodec.c qoi2img.c
	$(CC) $(CFLAGS) -c qoimg.c qoicodec.c qoi2img.c
	ar -cr libqoi.a qoimg.o qoicodec.o qoi2img.o
