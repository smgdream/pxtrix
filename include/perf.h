/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */
/* Depend on perf.c */

#ifndef PERF_H
#define PERF_H

#include <stdint.h>

/* unit of time: us */

/* Reset the timer.  */
void timer_reset(void);
/* Different of the time between prev call and this call (us).  */
uint64_t time_step(void);
/* The time total used after the timer start or reset (us).  */
uint64_t time_all(void);
/* Start the timer.  */
#define timer_start() timer_reset()

#endif
