# Render Library
Render组件包含了一些常用的用于绘制图形的功能和处理图像的功能。  

# 使用说明
使用时需包含`render.h`并且编译相关C源文件或链接有关库。  

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
/* draw a cubic bezier curve */
Image *bezier_curve_c(Image *img, Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, const Px_def *clr);
/* Resize image.  */
Image *scale(Image *img, uint32_t wid, uint32_t hgt);
/* Crop image.  */
Image *crop(Image *img, uint32_t x, uint32_t y, uint32_t wid, uint32_t hgt);
/* Montage or overlay image.  */
Image *cat(Image *dest, const Image *src, uint32_t x, uint32_t y);
/* Invert image.  */
Image *invert(Image *img);
/* Filp vertical */
Image *filp_v(Image *img);
/* Gamma correction */
double adj_gamma(double gamma, double x);
```

## 函数详细说明

```c
Image *gray(Image *img, uint8_t (*f)(const Px_def *px));
```
`gray`函数用于将`img`转为灰度图。返回`img`，如果发生错误则返回`NULL`。  
参数`f`是回调函数的函数指针，其用于让用户定义灰度化计算，当`f`的实参为`NULL`时将会使用内置的灰度化计算方式，执行时`gray`将会向`f`传递需要计算灰度值的像素的指针，`f`需要通过该像素计算出灰度值（其合理范围为 [0, 255]）并返回。  
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

```c
Image *bezier_curve_c(Image *img, Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, const Px_def *clr);
```
`bezier_curve_c`函数用于绘制一条三次贝塞尔曲线，`p0` ~ `p3`为控制贝塞尔曲线的锚点，`clr`用于指定线条颜色，返回`img`，若出错则返回`NULL`.  
<br>

```c
Image *scale(Image *img, uint32_t wid, uint32_t hgt);
```
`scale`调整图像大小（主要用于缩小），缩放`img`。返回`img`，若出错则返回NULL.
<br>

```c
Image *crop(Image *img, uint32_t x, uint32_t y, uint32_t wid, uint32_t hgt);
```
`crop`裁剪图像，选取`img`从(x, y)开始的宽`wid`高`hgt`的图像区域(i.e. (x, y)->(x+wid-1, y+hgt-1))。返回`img`，出错则返回`NULL`.
<br>

```c
Image *cat(Image *dest, const Image *src, uint32_t x, uint32_t y);
```
`cat`重叠或拼接图像，将源图像(`src`)重叠/拼接到目标图像上(`dest`)，源图像的原点在目标图像上的位置由参数(`x`, `y`)控制（底层图像（目标图像）和上层图像（源图像）均以对应图像的左下角像素为相对原点），源图像超出目标图像边界的部分将会被截断。返回`dest`，若出错则返回`NULL`.（待完善）
<br>

```c
Image *invert(Image *img);
```
`invert`图像颜色反转（即负片）。返回`img`，若出错则返回`NULL`.
<br>

```c
Image *filp_v(Image *img)
```
`filp_v`垂直翻转图像。返回`img`,出错则返回`NULL`.
<br>

```c
double adj_gamma(double gamma, double x);
```
`adj_gamma`伽马校正函数，`gamma`: 伽马值，`x`: 输入，`x`的定义域为\[0, 1\]。返回x^gamma, 其值域为\[0, 1\].
<br>

## Colour Lookup Table
Available at: [Color Lookup Table](lut.md)