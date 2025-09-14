# Tiny Mathematics Library

Tiny Mathematics 是一个轻量的数学库。其中定义了一些常用数学常量，数学（结构）类型和数学函数。  
Attention: tmath.h is not tgmath.h

## Usage

```c
#include "tmath.h"
```

## Constant
| Name		| Symbol	| Macro		| Value				|
| --------- | --------- | --------- | ----------------- |
| Pi		| π			| PI		| 3.141592652589	|
| Tau		| τ			| TAU		| 6.283185307179	|
| Euler		| e			| NC		| 2.718281828459	|
| Golden	| φ			| GOLD		| 1.618033988749	|
<!-- | 	| 	| 	| 	| -->

## Type defined
```c
typedef struct vec2 {
	double x;
	double y;
} Vec2;

typedef struct vec3 {
	double x;
	double y;
	double z;
} Vec3;
```

## Function and macro

```c
// tmath.h

double polar_x(double r, double t);
double polar_y(double r, double t);
double polar_r(double x, double y);
double polar_t(double x, double y);

polar_for_each(r, t, r_max)
```
```c
/* Factorial of n */
double factorial(double n);
/* Binomial coefficient */
double bin_coe(double n, double k);

/* linear interpolation of real scalar */
double lerp(double a, double b, double t);

/* signum */
double sgn(double x);
```
```c
/* Vector2 function */

double vec2_mod(Vec2 v);
Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_sub(Vec2 a, Vec2 b);
Vec2 vec2_mul(double c, Vec2 v);
/* scalar product */
double vec2_spro(Vec2 a ,Vec2 b);
/* linear interpolation for 2D vector */
Vec2 vec2_lerp(Vec2 a, Vec2 b, double t);
```
```c
/* bezier function */

/* simple bezier */
Vec2 bezier_s(Vec2 p0, Vec2 p1, double t);
/* quadratic bezier */
Vec2 bezier_q(Vec2 p0, Vec2 p1, Vec2 p2, double t);
/* cubic bezier */
Vec2 bezier_c(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, double t);
/* n-order bezier */
Vec2 bezier_n(Vec2 *p_arr, size_t n, double t);
```

## 详细说明

### Coordinate convertion

```c
double polar_x(double r, double t);
```
求极坐标(r, t)对应的直角坐标x分量的值。  
<br>

```c
double polar_y(double r, double t);
```
求极坐标(r, t)对应的直角坐标y分量的值。  
<br>

```c
double polar_r(double x, double y);
```
求直角坐标(x, y)对应的极坐标radius值。  
<br>

```c
double polar_t(double x, double y);
```
求直角坐标(x, y)对应的极坐标theta值。  
<br>

### Useful functions

```c
double factorial(double n);
```
n的阶乘。返回n!
<br>

```c
double bin_coe(double n, double k);
```
二项式系数。返回(n; k)
<br>

```c
double lerp(double a, double b, double t);
```
实数线性插值。返回a+t*(b-a)
<br>

```c
double sgn(double x);
```
实数符号函数。当x非零返回 x / |x|否则返回0  
<br>

### Functions for 2D vector

```c
double vec2_mod(Vec2 v);
```
二维向量的模。返回 |**v**|
<br>

```c
Vec2 vec2_add(Vec2 a, Vec2 b);
```
二维向量加法。返回 **a** + **b**
<br>

```c
Vec2 vec2_sub(Vec2 a, Vec2 b);
```
二维向量减法。返回 **a** - **b**
<br>

```c
Vec2 vec2_mul(double c, Vec2 v);
```
二维向量数乘。返回 c**v**
<br>

```c
double vec2_spro(Vec2 a ,Vec2 b);
```
二维向量标量积。返回 **a** · **b**
<br>

```c
Vec2 vec2_lerp(Vec2 a, Vec2 b, double t);
```
二维向量线性插值。返回**a** + t*(**b** - **a**)
<br>

### Bezier functions
```c
Vec2 bezier_s(Vec2 p0, Vec2 p1, double t);
```
线性贝塞尔函数。返回锚点为`p0`, `p1`时参数`t`经线性贝塞尔函数映射而得的向量。
<br>

```c
Vec2 bezier_q(Vec2 p0, Vec2 p1, Vec2 p2, double t);
```
二次贝塞尔函数。返回锚点为`p0`, `p1`, `p2`时参数`t`经二次贝塞尔函数映射而得的向量。
<br>

```c
Vec2 bezier_c(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, double t);
```
三次贝塞尔函数。返回锚点为`p0`, `p1`, `p2`, `p3`时参数`t`经三次贝塞尔函数映射而得的向量。
<br>

```c
Vec2 bezier_n(Vec2 *p_arr, size_t n, double t);
```
n次贝塞尔函数，`p_arr`为指向锚点数组的指针。返回锚点确定时参数`t`经n次贝塞尔函数映射而得的向量。
<br>

