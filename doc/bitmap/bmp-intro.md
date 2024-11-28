# Introduction of Bitmap Image Model

bmp模块用于对Microsoft Bitmap Image文件进行处理，如：bmp文件IO、与Image图像类型相互转换。该模块定义了一种`Bmpimg`图像类型用于存储bmp文件的数据，还定义了一系列用于操作Bmpimg类型的操作函数。

若要使用bmp模块，需要包含`bmpimg.h`头文件，并要编译相关的C语言源文件。

## Bitmap Image像素排列方式
```
y ^ 
  |
  |   bmp
  |
  +----------> 
0             x
```
坐标原点位于图像左下角，像素从左到右，从下到上，逐行排列。
像素从(0, 0)开始到(wid-1, hgt-1)结束。

## Bmpimg类型的结构定义
```c
/* bmpimg.h */

typedef struct bmpimg {
	Bmpfile_header bfh;
	Bmpinfo_header bih;
	Bmp_px_def *buf; // pixel buffer
} Bmpimg;
```
```c
typedef struct bmp_px_rgb24 {
	/* bmpimg.h */
	uint8_t b; // blue
	uint8_t g; // green
	uint8_t r; // red
} Bmp_px_rgb24, Bmp_px_def;
```
关于两个bmp header类型的定义见bmp-header文档。  

## Bmpimg操作函数(宏)
```c
/* bmpimg.h */
/* Basic function */

/* New a Bmpimg object */
Bmpimg *bmp_new(int32_t wid, int32_t hgt, uint16_t bpp);

/* Free a Bmpimg object */
int bmp_free(Bmpimg *bmp);

/* Read a bmp file and create a Bmpimg for it */
Bmpimg *bmp_read(const char *filename);

/* Write the bmp to a bmp file */
int bmp_write(const Bmpimg *bmp, const char *filename);

/* Convert a Bmpimg to a Image */
Image *bmp2img(const Bmpimg *bmp, Image *img);

/* Convert a Image to a Bmpimg */
Bmpimg *img2bmp(const Image *img, Bmpimg *bmp);

/* Print the info of bmp to a file stream */
int bmp_info(const Bmpimg *bmp, void *fileptr);

/* Be used to new a empty bmpimg object as an argument */
macro: BMP_EMPTY
```

<br>

```c
/* bmpimg.h */
/* Other function */


/* Reconfigure the Bmpimg object */
Bmpimg *bmp_set(Bmpimg *bmp, int32_t wid, int32_t hgt, uint16_t bpp);
/* Get the pixel at (x, y) from bmp */
Bmp_px_def *Bmp_px(const Bmpimg *bmp, int32_t x, int32_t y);
/* Get the real size of a line of pixels at bmp file */
static inline size_t bmp_sizeof_line(int32_t wid);
int bmpfile_set_header(Bmpfile_header *bfhp, int32_t wid, int32_t hgt);
int bmpinfo_set_header(Bmpinfo_header *bihp, int32_t wid, int32_t hgt, uint16_t bpp);
```

### Bmpimg操作函数使用说明

#### 基本函数
```c
Bmpimg *bmp_new(int32_t wid, int32_t hgt, uint16_t bpp);
```
`bmp_new`函数用于新建一个`Bmpimg`对象，返回指向该对象的指针若出错则返回NULL.  
`wid`为图像宽度px，`hgt`为图像高度px，`bpp`为图像的像素深度(bits per pixel)，可以用BMP_EMPTY宏代替所有参数创建一个空对象，如：`bmp_e = bmp_new(BMP_EMPTY);`  
<br>

```c
int bmp_free(Bmpimg *bmp);
```
`bmp_free`函数用于释放一个Bmpimg对象，出错时返回非0值。  
<br>

```c
Bmpimg *bmp_read(const char *filename);
```
`bmp_read`函数将读取一个bmp图像文件并为其创建一个Bmpimg对象。返回指向该对象的指针，若出错则返回NULL.  
<br>

```c
int bmp_write(const Bmpimg *bmp, const char *filename);
```
`bmp_write`函数用于将bmp指向的Bmpimg对象写入到指定bmp图像文件中。当出错时返回非0值。  
<br>

```c
Image *bmp2img(const Bmpimg *bmp, Image *img);
```
`bmp2img`函数将`Bmpimg`对象转换为`Image`对象。返回img，若出错则返回NULL.  
<br>

```c
Bmpimg *img2bmp(const Image *img, Bmpimg *bmp);
```
`img2bmp`函数将`Image`对象转换为`Bmpimg`对象。返回bmp，若出错则返回NULL.  
<br>

```c
int bmp_info(const Bmpimg *bmp, void *fileptr);
```
`bmp_info`函数将bmp的信息输出到文件流fileptr中。出错则返回非0值。  
<br>

#### 其它函数
```c
Bmpimg *bmp_set(Bmpimg *bmp, int32_t wid, int32_t hgt, uint16_t bpp);
```
`bmp_set`函数用于重新设置`bmp`，返回bmp，出错时则返回NULL.  
`wid`为图像宽度px，`hgt`为图像高度px，`bpp`为图像的像素深度(bits per pixel)，可以用BMP_EMPTY宏代替第一个参数之后的所有参数将一个`Bmpimg`对象设置为空对象，如：`bmp_noe = bmp_set(bmp_noe, BMP_EMPTY);`  
<br>

```c
Bmp_px_def *bmp_px(const Bmpimg *bmp, int32_t x, int32_t y);
```
`bmp_px`函数将返回bmp中位于(x, y)处的像素的指针，若出错则返回NULL.  
<br>

```c
static inline size_t bmp_sizeof_line(int32_t wid);
```
`bmp_sizeof_line`函数用于计算在bmp图像文件中的一行像素实际所占字节数。 
<br>

## 补充

### 关于空Bmpimg对象

可对空Bmpimg对象进行的操作的函数是受限的，其合法操作函数有: `bmp_free`, `img2bmp`, `bmp_info`, `bmp_set`以及其它用户自己设计的合理合法的操作函数。

## Know more

- [Portable Bitmap Header](https://github.com/smgdream/pxtrix/blob/master/doc/bitmap/bitmap-header.md)
- [MS Bitmap Image Format](https://github.com/smgdream/pxtrix/blob/master/doc/bitmap/ms-bmp-format.md)