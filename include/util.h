// SPDX-License-Identifier: MIT
/* Copyright (c) 2025 Smgdream */

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <time.h>
#include "perf.h"

/* Used to create a special literal.
 * The type of it is pointer to  object of TYPE.  */
#define obj_literal(TYPE, ...) ((TYPE []){__VA_ARGS__})

/* Sleeping in milliseconds */
int msleep(uint32_t millisec);

/* Writing byte sequence to temporary file. Returns FILE pointer, or NULL on
 * error. The position of the stream is the begin of the temporary file.
 * On Windows 7 or later system, if the program which using tmpfile function
 * executing on C driver a Permission denied error may occur.  */
FILE *byteseq2tmpfile(const uint8_t *seq, size_t size);

#endif