/* Licensed under the MIT License
 * Copyright (c) 2025 Smgdream */

#ifndef UTIL_H
#define UTIL_H

#include "perf.h"

/* Used to create a special literal.
 * The type of it is pointer to  object of TYPE.  */
#define obj_literal(TYPE, ...) ((TYPE []){__VA_ARGS__})

#endif