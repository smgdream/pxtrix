# Introdection of QOI Model

QOI模块用于对Quite Ok Image(QOI)图像格式进行IO、编解码、以及与Image图像格式进行转换。与该模块相关的图像类型是Qoimg. QOI模块还包含了对Qoimg类型进行操作的操作函数以及一些有关函数。  

要使用qoi模块需要包含`qoimg.h`头文件，并要编译相关C语言源文件或链接相关库。

## QOI的像素排列顺序
```
0             x
  +---------->
  |
  |   qoi
  |
y v
```
QOI图像以图像左上角为原点从左到右、从上到下、逐行排列，从(0, 0)开始到(wid-1, hgt-1)结束。

## Qoimg类型的结构定义

```c
/* qoimg.h */

typedef struct qoimg {
	Qoi_header  header;
	Qoi_px_def *buf;
} Qoimg;
```

```c
/* qoimg.h */
/* The defined of qoi_header structure */

typedef struct qoi_header {
	char magic[4];		// magic bytes "qoif"
	uint32_t width;		// image width in pixels (BE)
	uint32_t height;	// image height in pixels (BE)
	uint8_t channels;	// 3 = RGB, 4 = RGBA
	uint8_t colorspace;	// 0 = sRGB with linear alpha
						// 1 = all channels linear
} Qoi_header;
```

```c
/* qoimg.h */
/* The define of qoi_px_rgba structure */

typedef struct qoi_px_rgba {
	uint8_t r; // red
	uint8_t g; // green
	uint8_t b; // blue
	uint8_t a; // alpha
} Qoi_px_rgba, Qoi_px_def;
```

## Qoimg相关枚举
```c
enum qoi_channels_set { QOI_CH_RGB = 3, QOI_CH_RGBA = 4 };
enum qoi_colorspace_set {QOI_GAMUT_SRGB_LA = 0, QOI_GAMUT_ALL_LINEAR = 1 };
```

## Qoimg操作函数(宏)

```c
/* qoimg.h */
/* Basic function */

/* New a Qoimg object */
Qoimg *qoi_new(uint32_t wid, uint32_t hgt, uint8_t channels, uint8_t colorspace);
/* Free a Qoimg object */
int qoi_free(Qoimg *qoi);

/* Read and decode a QOI image file */
Qoimg *qoi_read(const char *filename);
/* Write and encode a Qoimg object to a QOI image file */
int qoi_write(const Qoimg *qoi, const char *filename);

/* Test a file is qoi file or not */
int qoi_valid(const char *filename);
/* Print the information of qoi to a file stream */
int qoi_info(const Qoimg *qoi, void *fileptr);

/* Use as a argument to create a empty Qoimg object */
#define QOI_EMPTY 0, 0, QOI_CH_RGBA, QOI_GAMUT_SRGB_LA

/* qoimg.h */
/* Other function */

/* Reconfigure a Qoimg object */
Qoimg *qoi_set(Qoimg *qoi, uint32_t wid, uint32_t hgt, uint8_t channels, uint8_t colorspace);
/* Get the pixel at (x, y) in Qoimg */
Qoi_px_def *qoi_px(const Qoimg *qoi, uint32_t x, uint32_t y);
/* Decode the qoi data chunks to rgba pixels */
int qoi_decode(Qoi_px_def *dest, const Byte *iobuf, uint64_t cnt);
/* Encode the rgba pixels data to data chunks */
size_t qoi_encode(Byte *iobuf, const Qoi_px_def *src, uint64_t cnt);

/* Read and decode a qoi image from a file stream */
Qoimg *qoi_read_stream(void *filestream);
/* Test a qoi stream is valid qoi stream or not */
int qoi_valid_stream(void *stream);
```

<br>

### Qoimg操作函数使用说明

#### 基本函数
```c
Qoimg *qoi_new(uint32_t wid, uint32_t hgt, uint8_t channels, uint8_t colorspace);
```
`qoi_new`函数用于新建一个`Qoimg`对象。返回该对象的指针，若出错则返回`NULL`。新建的`Qoimg`对象的画布颜色为透明的黑色。  
`wid`为图像宽度px，`hgt`为图像高度px，`channels`为图像通道数，`colorspace`为图像色域常数值。`QOI_EMPTY`宏可代替所有参数用以创建一个空白`Qoimg`对象。  
<br>

```c
int qoi_free(Qoimg *qoi);
```
`qoi_free`函数用于释放`Qoimg`对象，若出错则返回非0值。  
<br>

```c
Qoimg *qoi_read(const char *filename);
```
`qoi_read`函数用于读取并解码QOI图像文件并为其创建一个`Qoimg`对象。该函数返回指向该对象的指针，当发生错误则返回`NULL`.  
<br>

```c
int qoi_write(const Qoimg *qoi, const char *filename);
```
`qoi_write`函数用于将`qoi`编码并写入的QOI文件中，若出错则返回非0值。  
<br>

```c
int qoi_info(const Qoimg *qoi, void *fileptr);
```
`qoi_info`将`qoi`的信息打印到文件流中，若出错返回非0值。  
<br>

```c
int qoi_valid(const char *filename);
```
`qoi_valid`函数用于测试文件(`filename`)是否为有效的QOI文件，是则返回真（非0值）否则返回假 (0)。  

#### 其它函数
```c
Qoimg *qoi_set(Qoimg *qoi, uint32_t wid, uint32_t hgt, uint8_t channels, uint8_t colorspace);
```
`qoi_set`函数用于重新设置`qoi`，返回`qoi`，若出错则返回`NULL`.  
宏`QOI_EMPTY`可用于代替第一个参数之后的所有参数，用以创建一个空白Qoimg对象。  

如果图像宽度或高度发生了改变，则画面内容是由实现定义的。换言之，不要对此时的画面内容作任何假设。  
<br>

```c
Qoi_px_def *qoi_px(const Qoimg *qoi, uint32_t x, uint32_t y);
```
`qoi_px`函数用于获得在指定Qoimg对象中位于坐标(x, y)的像素点。返回对应像素点的指针，若出错则返回`NULL`.  
<br>

```c
int qoi_decode(Qoi_px_def *dest, const Byte *iobuf, uint64_t cnt);
```
`qoi_decode`函数用于将原始的字节数据解码为rgba像素。正常返回0，若出错则返回非0值。  
`dest`指向用于存储解码出来的rgba像素数据的缓冲区，`iobuf`为存储未解码字节数据的IO缓冲区，`cnt`是要被解码的像素数量。  
<br>

```c
size_t qoi_encode(Byte *iobuf, const Qoi_px_def *src, uint64_t cnt);
```
`qoi_encode`函数用于将rgba像素编码为字节数据。返回编码而得的字节数，若出错则返回0.  
`iobuf`为存储已编码字节数据的IO缓冲区，`src`指向待编码的rgba数据缓冲区，`cnt`是要被编码的像素数量。
<br>

```c
Qoimg *qoi_read_stream(void *filestream);
```
`qoi_read_stream`从文件流中读取一个QOI图像文件并将其解码再为其创建一个Qoimg对象，返回指向该对象的指针，若出错返回NULL。`filestream`必须指向一个有效QOI文件数据段的开头。在读流操作完成后该函数会将文件位置设置为文件的开头。  
<br>

```c
int qoi_valid_stream(void *stream);
```
`qoi_valid_stream`函数用于测试一个流是否是一个有效的qoi流，如果有效返回`true`无效则返回`false`。`stream`应当指向一个qoi数据段的开头，本函数不会改变流对应的文件位置<!-- 实际上该函数测试流是会改变流的位置的。但是在读流之前该函数会保存流的文件位置。而在函数返回之前，流的文件位置将会设回原位。对于本函数的用户来说，这效果就等同于“本函数不会改变流对应的文件位置” -->。  
<br>

## 补充

### 空白Qoimg对象

可对空Qoimg对象进行的操作的函数是受限的，其合法操作函数有: `qoi_free`, `qoi_info`, `qoi_set`以及其它用户自己设计的合理合法的操作函数。  

## Know more
- [QOI Specification v1.0](qoi-spec.md)
- [QOI Specification v1.0 zh_CN (Translate by Smgdream)](qoi-spec_chs.md)