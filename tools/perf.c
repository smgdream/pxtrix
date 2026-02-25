/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

// linux: cc thisfile.c -lrt

#ifdef __unix__
	/* Must be before everything. But I don't know why. */
	#define _POSIX_C_SOURCE 199309L
#endif

#include <stdint.h>
#include "perf.h"

#ifdef __unix__
	#include <time.h>
#else
	#include <profileapi.h>
#endif


/* unit of time is us */

typedef uint64_t usec_t;
static usec_t beg = 0, last = 0;

#ifdef __unix__
static inline usec_t phy_clock(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}
#else
static usec_t freq_per_us;
static LARGE_INTEGER freq;

static inline usec_t phy_clock(void)
{
	LARGE_INTEGER tick;

	QueryPerformanceCounter(&tick);
	return (usec_t) tick.QuadPart/freq_per_us;
}
#endif

void timer_reset(void)
{
#ifdef _WIN64
	QueryPerformanceFrequency(&freq);
	freq_per_us = (usec_t) (freq.QuadPart / 1000000);
#endif

	beg = last = phy_clock();
	return;
}

uint64_t time_step(void)
{
	usec_t rt = 0, dt = 0;

	rt = phy_clock();
	dt = rt - last;
	last = rt;
	return (uint64_t) dt;
}

uint64_t time_all(void)
{
	return (uint64_t) (phy_clock() - beg); // dt = rt - beg
}

/* Reference:
 * UNIX:
 * [1] Advanced Programming in the UNIX Environment, 3rd Edition Section 4.2 p.94
 * [2] Advanced Programming in the UNIX Environment, 3rd Edition Section 6.10 p.189
 * [3] The Linux Programming Interface Section 3.6
 * [4] The Linux Programming Interface Section 23.5
 * [5] IEEE/Open Group Standard for Information Technology--Portable Operating System
 * 	   Interface (POSIX™) Base Specifications, Issue 8 Vol. 1: Base Definitions
 * 	   Chapter 14 (Headers) <time.h> p.452-453
 * Windows
 * [6] Acquiring high-resolution time stamps. Available: https://learn.microsoft.com/en-us/
 * 	   windows/win32/sysinfo/acquiring-high-resolution-time-stamps
 * [7] QueryPerformanceCounter function (profileapi.h). Available: https://learn.microsoft.com/
 * 	   en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter
 * [8] QueryPerformanceFrequency function (profileapi.h). Available: https://learn.microsoft.com/
 * 	   en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency
 * [9] LARGE_INTEGER union (winnt.h). Available: https://learn.microsoft.com/en-us/windows/
 * 	   win32/api/winnt/ns-winnt-large_integer-r1
 */

