/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

/* Tiny mathematics */

#ifndef TMATH_H
#define TMATH_H

#include <stddef.h>
#include <math.h>

#define PI (3.141592652589)
#define TAU (6.283185307179)
/* Nautical constant */
#define NC (2.718281828459)
#define GOLD (1.618033988749)

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max3(a, b, c) (max((a), max((b), (c))))
#define min3(a, b, c) (min((a), min((b), (c))))

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
 * (x, y). Returns value in range [0, +inf).  */
static inline double polar_r(double x, double y)
{
	return sqrt(x * x + y * y);
}

/* Determine the polar coordinate t corresponding to Cartesian coordinates
 * (x, y). Return value in range [0, 2Pi) */
static inline double polar_t(double x, double y)
{
	double t = atan2(y, x);
	return (t >= 0) ? t : t + TAU;
}

/* range the polar coordinate r (radius) is a double iterator, t (theta) is
 * a double iterator  */
#define polar_for_each(r, t, r_max) \
	for ((r) = 0.0; r <= (r_max); (r) += 0.1) \
		for ((t) = 0.0; (t) < TAU; (t) += 0.001)

/* Factorial of n. Returns n! */
static inline double factorial(double n)
{
	double x = 1;

	if (n == 0 || n == 1)
		return 1.0;
	while (n > 1)
		x *= n--;
	return n;
}

/* binomial coefficient */
static inline double bin_coe(double n, double k)
{
	return factorial(n) / (factorial(k) * factorial(n-k));
}

/* linear interpolation of real scalar */
static inline double lerp(double a, double b, double t) 
{
	return a + t * (b-a);
}

/* signum */
static inline double sgn(double x)
{
	return (x > 0) ?
				1 :
			(x < 0) ?
				-1 :
				0;
}

/* Vectors */

typedef struct vec2 {
	double x;
	double y;
} Vec2;

typedef struct vec3 {
	double x;
	double y;
	double z;
} Vec3;

/* |v| */
static inline double vec2_mod(Vec2 v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

/* a+b */
static inline Vec2 vec2_add(Vec2 a, Vec2 b)
{
	Vec2 result = { 0, 0 };
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return result;
}

/* a-b */
static inline Vec2 vec2_sub(Vec2 a, Vec2 b)
{
	Vec2 result = { 0, 0 };
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return result;
}

/* scalar multiplication of 2D vector */
static inline Vec2 vec2_mul(double c, Vec2 v)
{
	Vec2 u = { 0, 0 };

	u.x = c * v.x;
	u.y = c * v.y;
	return u;
}

/* scalar product */
static inline double vec2_spro(Vec2 a, Vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

/* linear interpolation of 2D vector. returns a + t*(b-a) */
static inline Vec2 vec2_lerp(Vec2 a, Vec2 b, double t) 
{
	// a + t(b-a)
	return vec2_add(a, vec2_mul(t, vec2_sub(b, a)));
}

/* simple bezier */
static inline Vec2 bezier_s(Vec2 p0, Vec2 p1, double t)
{
	return vec2_lerp(p0, p1, t);
}

/* quadratic bezier */
static inline Vec2 bezier_q(Vec2 p0, Vec2 p1, Vec2 p2, double t)
{
	/* B(t) = (1-t)^2*p0 + 2t(1-t)p1 + t^2p2 */
	Vec2 a = vec2_mul((1-t)*(1-t), p0);
	Vec2 b = vec2_mul(2*t*(1-t), p1);
	Vec2 c = vec2_mul(t*t, p2);
	
	return vec2_add(vec2_add(a, b), c);
}

/* cubic bezier */
static inline Vec2 bezier_c(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, double t)
{
	/* B(t) = (1-t)^3*p0 + 3t(1-t)^2*p1 + 3t^2(1-t)p2 + t^3p3 */
	Vec2 a = vec2_mul((1-t)*(1-t)*(1-t), p0);
	Vec2 b = vec2_mul(3*t*(1-t)*(1-t), p1);
	Vec2 c = vec2_mul(3*t*t*(1-t), p2);
	Vec2 d = vec2_mul(t*t*t, p3);
	
	return vec2_add(d, vec2_add(c, vec2_add(b, a)));
}

/* n-order bezier. p_arr is a pointer to a array of control points.  */
static inline Vec2 bezier_n(Vec2 *p_arr, size_t n, double t)
{
	Vec2 sum = { 0, 0 };
	/* B(t) = sum(i = 0, n)p[i](1-t)^(n-i)t^i */
	for (size_t i = 0; i <= n; ++i)
		sum = vec2_add(sum, vec2_mul( bin_coe(n, i) * pow((1-t), n-i) * pow(t, i), p_arr[i]));
	
	return sum;
}

#endif
