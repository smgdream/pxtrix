/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#include <time.h>
#include "perf.h"

/* unit of time is us */

static clock_t beg = 0, last = 0;

void timer_reset(void)
{
	beg = last = clock();
	return;
}

uint32_t time_step(void)
{
	clock_t rt = 0, dt = 0;

	rt = clock();
	dt = rt - last;
	last = rt;
	return (uint32_t) (dt * 1000000 / CLOCKS_PER_SEC);
}

uint64_t time_all(void)
{
	clock_t rt = 0, dt = 0;

	rt = clock();
	dt = rt - beg;
	return (uint64_t) (dt * 1000000 / CLOCKS_PER_SEC);
}
