# Tiny Mathematics Library

Tiny Mathematics (tmath.h) 是一个轻量的数学库。其中定义了一些常用数学常量和数学函数。  
Attention: tmath.h is not tgmath.h

## Constant
| Name		| Symbol	| Macro		| Value				|
| --------- | --------- | --------- | ----------------- |
| Pi		| π			| PI		| 3.141592652589	|
| Tau		| τ			| TAU		| 6.283185307179	|
| Euler		| e			| E			| 2.718281828459	|
| Golden	| φ			| GOLDEN	| 1.618033988749	|
<!-- | 	| 	| 	| 	| -->

## Function and Macro

```C
// tmath.h

double polar_x(double r, double t);
double polar_y(double r, double t);
double polar_r(double x, double y);
double polar_t(double x, double y);
```
```
// tmath.h
polar_for_each(r, t, r_max)
```

## 详细说明

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
