# Pic to Image

该模块用于Image图像类型的对象和其他图像类型对象的相互转换。

## 使用说明

使用时需包含`pic2img.h`并编译相应的C源文件或链接相应的库。

## 函数声明
```c
/* Convert a Bmpimg to a Image */
Image *bmp2img(const Bmpimg *bmp, Image *img);
/* Convert a Image to a Bmpimg */
Bmpimg *img2bmp(const Image *img, Bmpimg *bmp);
```
```c
/* Convert a Qoimg object to an Image object */
Image *qoi2img(const Qoimg *qoi, Image *img);
/* Convert an Image object to a Qoimg object */
Qoimg *img2qoi(const Image *img, Qoimg *qoi);
```
```c
/* Convert Pgm object to Image object */
Image *pgm2img(const Pgm *p5, Image *img);
/* Convert Image object to pgm object */
Pgm *img2pgm(const Image *img, Pgm *p5, uint8_t (*gray)(const Px_def *px));
```
```c
/* Convert the Fbdev object to Image object */
Image *fb2img(const Fbdev *fbd, Image *img);
/* Convert the Image object to Fbdev object */
Fbdev *img2fb(const Image *img, Fbdev *fbd);
```
```c
/* Convert a Image object to Chart object */
Chart *img2ca(const Image *img, Chart *ca);
```

### 详细说明

```c
Image *bmp2img(const Bmpimg *bmp, Image *img);
```
`bmp2img`函数将`Bmpimg`对象转换为`Image`对象。返回img，若出错则返回NULL.  
注：如果`Image`对象的像素缓冲区大小不合适，则重新调整像素缓冲区大小（像素缓冲区的地址可能会发生变化）。
```c
Bmpimg *img2bmp(const Image *img, Bmpimg *bmp);
```
`img2bmp`函数将`Image`对象转换为`Bmpimg`对象。返回bmp，若出错则返回NULL. 
注： 如果`Bmpimg`对象的像素缓冲区大小不合适，则重新调整像素缓冲区大小（像素缓冲区的地址可能会发生变化）。  
<br>


```c
Image *qoi2img(const Qoimg *qoi, Image *img);
```
`qoi2img`函数将一个`Qoimg`对象转换为`Image`对象。返回`img`，若出错则返回`NULL`.  
注：如果`Image`对象的像素缓冲区大小不合适，则重新调整像素缓冲区大小（像素缓冲区的地址可能会发生变化）。  

```c
Qoimg *img2qoi(const Image *img, Qoimg *qoi);
```
`img2qoi`函数将一个`Image`对象转换为`Qoimg`对象。返回`qoi`，若出错则返回`NULL`.  
注：如果`Qoimg`对象的像素缓冲区大小不合适，则重新调整像素缓冲区大小（像素缓冲区的地址可能会发生变化）。  
<br>


```c
Image *pgm2img(const Pgm *p5, Image *img);
```
`pgm2img`函数用于将Pgm对象转换为Image对象。返回img，如果则出错返回NULL。  
注：如果`Image`对象的像素缓冲区大小不合适，则重新调整像素缓冲区大小（像素缓冲区的地址可能会发生变化）。  

```c
Pgm *img2pgm(const Image *img, Pgm *p5, uint8_t (*gray)(const Px_def *px))
```
`img2pgm`函数用于将Image对象转换为Pgm对象。返回p5，如果出错则返回NULL。  
`gray`接受一个函数指针用作回调函数，该回调函数需要一个类型为`const Px_def *`的参数，该参数向回调函数传递了要被灰度化处理的像素的指针，该函数需要向主调函数返回计算得出的一个[0, 255]的灰度值，如果传递给gray的实参是`NULL`则使用pgm模块内置的灰度计算函数计算灰度值。  
注：如果`Pgm`对象的像素缓冲区大小不合适，则重新调整像素缓冲区大小（像素缓冲区的地址可能会发生变化）。  
<br>


```c
Image *fb2img(const Fbdev *fbd, Image *img);
```
`fb2img`将`Fbdev`对象转换为`Image`对象，返回`img`，出错则返回`NULL`.  
注：如果`img`指向的对象的像素缓冲区大小不合适，将会重新设置像素缓冲区大小（像素缓冲区的地址可能会发生变化）。  

```c
Fbdev *img2fb(const Image *img, Fbdev *fbd);
```
`img2fb`将`Image`对象转换为`Fbdev`对象，返回`fbd`，若出错则返回`NULL`.  
注：如果`fbd`指向的对象的像素缓冲区大小不合适，将会重新设置其像素缓冲区大小（像素缓冲区的地址可能会发生变化）。
<br>


```c
Chart *img2ca(const Image *img, Chart *ca);
```
`img2ca`将Image对象转换为Chart对象。返回`ca`，出错则返回`NULL`.  
注：如果Chart对象的像素缓冲区大小不合适，则会将其自动调整至合适大小（像素缓冲区的地址可能会发生变化）。
<br>