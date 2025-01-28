# Render Library
Render组件包含了一些常用的用于绘制图形的功能和处理图像的功能。  

## 函数概览

```c
// render.h

/* Convert an RGBA image to grayscale */
Image *gray(Image *img, uint8_t (*f)(const Px_def *px));
/* Image thersholding */
Image *bin(Image *img, uint8_t threshold);
/* Fill the canvas with specify color */
Image *fill(Image *img, const Px_def *clr);
/* draw a line parallel to x axis */
Image *line_row(Image *img, uint32_t y, uint32_t weight, const Px_def *clr);
/* draw a line parallel to y axis */
Image *line_col(Image *img, uint32_t x, uint32_t weight, const Px_def *clr);
```

## 函数详细说明

```c
Image *gray(Image *img, uint8_t (*f)(const Px_def *px));
```
`gray`函数用于将`img`转为灰度图。返回`img`，如果发生错误则返回`NULL`。  
参数`f`是回调函数的函数指针，其用于让用户定义灰度化计算，当`f`的实参为`NULL`时将会使用内置的灰度化计算方式，执行时`gray`将会向`f`传递有关需要计算灰度值的像素的指针，`f`需要通过该像素计算出灰度值（其合理范围为 [0, 255]）并返回。  
<br>

```c
Image *bin(Image *img, uint8_t threhold);
```
`bin`函数用于对`img`进行二值化处理，`threhold`为阈值。返回`img`，如果发生错误则返回`NULL`。  
<br>

```c
Image *fill(Image *img, const Px_def *clr);
```
`fill`函数用于将`img`填充为指定的单一颜色`clr`。返回`img`，如果发生错误则返回`NULL`。  
<br>

```c
Image *line_row(Image *img, uint32_t y, uint32_t weight, const Px_def *clr);
```
`line_row`函数用于将`img`中绘制遵循数学代数式 *y* = y<sub>arg</sub> 的一条平行于 x 轴的直线，线径为 `weight`(px)，线条颜色通过`clr`指定。返回`img`，如果发生错误则返回`NULL`。  
<br>

```c
Image *line_col(Image *img, uint32_t x, uint32_t weight, const Px_def *clr);
```
`line_col`函数用于将img中绘制遵循数学代数式 *x* = x<sub>arg</sub> 的一条平行于 y 轴的直线，线径为 `weight`(px)，线条颜色通过`clr`指定。返回`img`，如果发生错误则返回`NULL`。  
<br>

## Colour Lookup Table
Available at: [Color Lookup Table](./lut.md)