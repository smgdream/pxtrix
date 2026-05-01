# SPDX-License-Identifier: MIT0

mdb_intact.run: main.c ctl.c ctl.h
	$(CC) -Wall -Wextra -std=c99 -O3 -I ../../../build/include -o $@ main.c ctl.c ../../../build/lib/*.a -lm