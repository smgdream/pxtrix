/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

/* Tiny mathematics */

#ifndef TMATH_H
#define TMATH_H

#include <math.h>


#define PI (3.141592652589)
#define TAU (6.283185307179)
#define E (2.718281828459)
#define GOLDEN (1.618033988749)

#ifndef __TINYC__
	/* Determine the Cartesian x-coordinate for polar coordinates (r, t).  */
	static inline double polar_x(double r, double t)
	{
		return r * cos(t);
	}

	/* Determine the Cartesian y-coordinate for polar coordinates (r, t).  */
	static inline double polar_y(double r, double t)
	{
		return r * sin(t);
	}

	/* Determine the polar coordinate r corresponding to Cartesian coordinates
	 * (x, y).  */
	static inline double polar_r(double x, double y)
	{
		return sqrt(x * x + y * y);
	}

	/* Determine the polar coordinate t corresponding to Cartesian coordinates
	 * (x, y).  */
	static inline double polar_t(double x, double y)
	{
		return atan2(y, x);
	}
#else
	#define polar_x(r, t) ((r) * cos(t))
	#define polar_y(r, t) ((r) * sin(t))
	#define polar_r(x, y) (sqrt((x) * (x) + (y) * (y)))
	#define polar_t(x, y) (atan2(y, x))
#endif
/* range the polar coordinate r (radius) is a double iterator, t (theta) is
 * a double iterator  */
#define polar_for_each(r, t, r_max) \
	for ((r) = 0.0; r <= (r_max); (r) += 0.1) \
		for ((t) = 0.0; (t) < TAU; (t) += 0.001)

#endif