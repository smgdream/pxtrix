/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

#ifndef UTIL_H
#define UTIL_H

#include "perf.h"

/* Used to create a special literal.
 * The type of it is pointer to  object of TYPE.  */
#define obj_literal(TYPE, ...) ((TYPE []){__VA_ARGS__})

#define	MS_PER_SEC (1000) 
#define US_PER_SEC (1000 * 1000)
#define US_PER_MS (1000)
#define us2sec(us) ((us) / US_PER_SEC)
#define us2ms(us) ((us) / US_PER_MS)
#define ms2sec(ms) ((ms) / MS_PER_SEC)

#endif