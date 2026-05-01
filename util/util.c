// SPDX-License-Identifier: MIT
/* Copyright (c) 2026 Smgdream */

#ifdef __unix__
	/* Must before everything */
	#define _POSIX_C_SOURCE 199309L
#endif

#include <stdio.h>
#include <stdint.h>

#ifdef __unix__
	#include <time.h>
#else /* Windows */
	#ifndef __TINYC__
		#include <IntSafe.h>
		#include <synchapi.h>
	#else
		#include <windows.h>
	#endif
#endif


int msleep(uint32_t ms)
{
#ifdef __unix__
	/* We can't try to write the initializer as {ms / 1000, ms % 1000 * 1000000}
	 * because POSIX only guarantee struct timespec included members tv_sec and 
	 * tv_nsec but it may has other field. This fields may locale in everywhere. 
	 * Such as the begin of the structure. In this situation, if we write the 
	 * initializer as {ms / 1000, ms % 1000 * 1000000} it may initialize other 
	 * field rather then tv_sec and tv_nsec. 
	 * 
	 * Description in POSIX Issue 8 Chapter 14 (Headers) <time.h> p. 452:
	 * The <time.h> header shall declare the timespec structure, which shall 
	 * include at least the following members: 
	 * time_t tv_sec Whole seconds. 
	 * long tv_nsec Nanoseconds [0, 999 999 999]. */
	struct timespec tp = {.tv_sec = ms / 1000, .tv_nsec = ms % 1000 * 1000000 };

	return nanosleep(&tp, NULL);
#else /* Windows */
	Sleep((DWORD) ms);
	return 0;
#endif
}

FILE *byteseq2tmpfile(const uint8_t *seq, size_t size)
{
	FILE *fp = NULL;
	/* On Windows 7 or later system, if the program which using tmpfile function 
	 * executing on C driver a Permission denied error will occur. Unless run as
	 * administrator. So fuck you Microsoft.
	 * ref: https://stackoverflow.com/questions/6247148/tmpfile-on-windows-7-x64
	 *		https://en.cppreference.com/c/io/tmpfile */
	if ((fp = tmpfile()) == NULL)
		return NULL;

	fwrite(seq, size, 1, fp);
	rewind(fp);

	return fp;
}