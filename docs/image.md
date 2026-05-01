# Image

Image模块包括：Image图像类型、Image图像类型操作函数(宏)、有关常量定义。
Image是一种与实际图像文件无关的一种通用图像交换格式，其被用于对图像数据进行渲染及处理。  

使用Image模块时需要包含`image.h`头文件，并编译`image.c`源文件或使用相应的库文件。

## Image像素排列方式
```
y ^ 
  |
  |  img px
  |
  +----------> 
0             x
```
坐标原点位于图像左下角，像素从左到右，从下到上，逐行排列。
像素从(0, 0)开始到(width-1, height-1)结束。

## Image结构定义

Image结构
```c
/* image.h */

typedef struct image {
	uint32_t width;  // width of image in px
	uint32_t height; // height of image in px
	uint32_t bpp;    // bits per pixel
	uint32_t gamut;  // color gamut
	Px_def	*buf;    // pixel buffer
} Image;
```
`Px_def`为默认像素结构，其定义在Pixel子模块的头文件pixel.h中，当前其对应RGBA32位浮点通道格式的像素。除此之外Pixel模块还定义了其他一些像素结构以及与像素相关的函数和宏。详见[Pixel模块](pixel.md)。

## image其它定义

色域枚举
```c
/* sRGB: IEC 61966-2-1
 * opRGB: IEC 61966-2-5 compatible with Adobe RGB 1998
 * BT.2020: ITU-R BT.2020-2
 * ACEScg: ACEScg Specification - ACEScg - A Working Space for CGI Render and
 * Compositing. Note: ACEScg is linear color space
 * UNO_LINEAR: Unkonwn color space with linear transfer function 
 * UNO_NON_LINEAR: Unkonwn color space with non-linear transfer function */
enum img_color_gamut { SRGB, OPRGB, BT_2020, ACESCG, UNO_NON_LINEAR, UNO_LINEAR };
```

## Image操作函数(宏/语句)

```c
/* image.h */

/* New a Image object.  */
Image *img_new(uint32_t wid, uint32_t hgt, uint32_t bpp, uint32_t gamut);
/* Free a Image object.  */
int img_free(Image *img);

/* Reconfigure a Image object.  */
Image *img_set(Image *img, uint32_t wid, uint32_t hgt, uint32_t bpp, uint32_t gamut);
/* Copy the date of Image object from src to dest.  */
Image *img_copy(Image *dest, const Image *src);
/* Get the pixel at (x, y) from img.  */
Px_def *img_px(Image *img, uint32_t x, uint32_t y);

/* Print the information of a Image object to the stream.  */
int img_info(const Image *img, void *fileptr);
/* Image object is empty or not.  */
int img_is_empty(const Image *img);

/* For x_name, y_name from (x_beg, y_beg) to (x_beg+wid-1, y_beg+hgt-1).  */
img_for_px(x_name, y_name, x_beg, y_beg, wid, hgt)
	statement;

/* Used as the argument of img_new, to new a empty Image object.  */
macro: IMG_EMPTY
```
为了保持模块的简洁与松耦合，其它图像类型与Image图像类型的双向转换由一个独立的模块(pic2img)实现，而并非由Image模块实现。  

### Image操作函数使用说明

```c
Image *img_new(uint32_t wid, uint32_t hgt, uint32_t bpp, uint32_t gamut);
```
`img_new`函数用于新建一个`Image`对象，返回pointer to Image，当发生异常则返回NULL。新建的Image对象的画布颜色为透明黑。  
wid为图像的宽度px，hgt为图像的高度px，bpp为图像的像素深度(bits per pixel)，gamut为图像的色域。`IMG_EMPTY`可用于代替该函数的一系列参数来创建一个空白`Image`对象，例如：`img_new(IMG_EMPTY)`  
<br>

```c
int img_free(Image *img);
```
`img_free`函数用于释放`Image`对象`img`，正常返回0，异常返回非0值。  
<br>

```c
Image *img_set(Image *img, uint32_t wid, uint32_t hgt, uint32_t bpp, uint32_t gamut);
```
`img_set`函数用于重新设置`img`。返回`img`，当发生异常则返回NULL。如果新像素所需缓冲区的空间大小等于原像素缓冲区所占空间，则不会重新分配像素缓冲区，反之亦然。还要注意，一旦图像的宽高发生了改变则原有的画面不再可用。此时画面的内容是由实现定义的，不要对画面内容作任何假设。  
wid为图像的宽度px，hgt为图像的高度px，bpp为图像的像素比特数(bits per pixel)，gamut为图像的色域。`IMG_EMPTY`可用于代替该函数的第一个参数后的一系列参数将`img`设置为空白`Image`对象。例如：`img_set(my_img, IMG_EMPTY)`  
<br>

```c
Image *img_copy(Image *dest, const Image *src);
```
`img_copy`函数用于复制`Image`对象，其将`src`的数据复制到`dest`中。返回`dest`，当出错时返回NULL。如有需要该函数会自动调整像素缓冲区对象。  
注：`src`或`dest`对应的实参可以为指向空白Image对象的指针。  
<br>

```c
Px_def *img_px(const Image *img, uint32_t x, uint32_t y);
```
`img_px`将返回`img`中位于(x, y)处的像素的指针，如果出错则返回NULL.  
<br>

```c
int img_info(const Image *img, void *fileptr);
```
`img_info`将`img`对象的信息输出到流`fileptr`中.
<br>

```c
int img_is_empty(const Image *img);
```
如果`img`指向的Image对象为空对象返回“真”否则返回“假”。  
<br>

```c
img_for_px(x_name, y_name, x_beg, y_beg, wid, hgt)
	statement;
```
`img_for_px`是一个宏，用于将已声明过的坐标指示变量`x_name`和`y_name`从(x_beg, y_beg)开始，迭代宽`wid`高`hgt`的矩形区域，迭代到(x_beg + wid - 1, y_beg + hgt - 1)，将会迭代`wid`*`hgt`个坐标。示例：
```c
uint32_t x = 0, y = 0;
img_for_px(x, y, 100, 50, 100, 50)
	*img_px(my_img, x, y) = (Px_def){0, 0, 0, 1};
```
这段代码将my_img从(100, 50)到(199, 99)的 100x50 的区域填充为黑色。

### 空白Image对象
空白Image对象没有像素缓冲区，其可执行的操作是受限的，可使用空白Image的合法操作函数有`img_free`, `img_set`, `img_copy`, `img_info`以及其他用户定义的合理的函数。

## Know More
- [Pixel](pixel.md)