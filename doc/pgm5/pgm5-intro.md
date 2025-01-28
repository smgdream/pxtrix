# Introduction of Portable Gray Map

pgm模块用于处理portable gray map (PGM)图像文件，其定义了用于存储PGM文件数据的`Pgm`结构。该模块主要负责PGM文件的读写及`Pgm`对象与`Image`对象的相互转换。  
portable gray map 图像文件是一种灰度图像文件，按照文件规范的定义其最大可支持65536阶灰阶，但本实现目前仅支持256阶灰阶。  
注：虽说PGM实际上有两种类型P2和P5，但是本pgm模块仅用于处理P5类型的PGM文件。  

## Portable Gray Map的像素排列顺序
```
0             x
  +---------->
  |
  |   pgm
  |
y v
```

## Pgm结构定义
```C
typedef struct pgm {
	uint16_t width;  // width of pgm image
	uint16_t height; // height of pgm image
	uint16_t maxval; // max value of a pixel of pgm image
	Pgm_px_def *buf; // pixel buffer of pgm image
} Pgm;
```

```C
typedef uint8_t Pgm_px_def;
```

## Pgm操作函数(宏)
```C
// pgm.h


/* basic operating functions of pgm */

/* New a pgm object */
Pgm *pgm_new(uint16_t width, uint16_t height);
/* Free a pgm object */
int pgm_free(Pgm *p5);

/* Read a pgm file and create a Pgm object for it */
Pgm *pgm_read(const char *filename);
/* Write a pgm file */
int pgm_write(const char *filename);

/* Convert Pgm object to Image object */
Image *pgm2img(const Pgm *p5, Image *img);
/* Convert Image object to pgm object */
Pgm *img2pgm(const Image *img, Pgm *p5, uint8_t (*gray)(const Px_def *px));

/* Print the information of a Pgm object to specified file by the file pointer fileptr */
int Pgm_info(const Pgm *p5, void *fileptr);
/* Test a file is pgm file or not */
int pgm_test(const char *filename);
/* Be used to new or re-set pgm object empty */
macro: PGM_EMPTY


/* extended operating functions of pgm */

/* Re-set a pgm object */
Pgm *pgm_set(Pgm *p5, uint16_t width, uint16_t height);
```

## Pgm操作函数详述

### pgm基本操作函数

```C
Pgm *pgm_new(uint16_t width, uint16_t height);
```
`pgm_new`函数用于通过参数width(宽度)和height(长度)创建对应规格的Pgm对象，返回指向该对象的指针，但如果出错则返回NULL。  
<br>

```C
int pgm_free(Pgm *p5);
```
`pgm_free`函数用于释放`Pgm`对象，正常返回0异常返回非0值。  
<br>

```C
Pgm *pgm_read(const char *filename);
```
`pgm_read`函数用于读取PGM图像文件并为其创建Pgm对象，返回指向该对象的指针，当发生错误时返回NULL。  
<br>

```C
int pgm_write(const Pgm *p5, const char *filename);
```
`pgm_write`函数用于将`Pgm`对象的内容写到PGM图像文件中，正常返回0异常返回非0值。  
<br>

```C
Image *pgm2img(const Pgm *p5, Image *img);
```
`pgm2img`函数用于将Pgm对象转换为Image对象。返回img，如果则出错返回NULL。  
注：如果`Image`对象的像素缓冲区大小不合适，则重新调整像素缓冲区大小（像素缓冲区的地址可能会发生变化）。  
<br>

```C
Pgm *img2pgm(const Image *img, Pgm *p5, uint8_t (*gray)(const Px_def *px))
```
`img2pgm`函数用于将Image对象转换为Pgm对象。返回p5，如果出错则返回NULL。  
`gray`接受一个函数指针用作回调函数，该回调函数需要一个类型为`const Px_def *`的参数，该参数向回调函数传递了要被灰度化处理的像素的指针，该函数需要向主调函数返回计算得出的一个[0, 255]的灰度值，如果传递给gray的实参是`NULL`则使用pgm模块内置的灰度计算函数计算灰度值。  
注：如果`Pgm`对象的像素缓冲区大小不合适，则重新调整像素缓冲区大小（像素缓冲区的地址可能会发生变化）。  
<br>

```C
int pgm_info(const Pgm *p5, void *fileptr);
```
`pgm_info`函数用于将显示`Pgm`对象的信息，正常返回0异常返回非0值。
注：`fileptr`实际是一个文件指针(`FILE *`)。  
<br>

```C
int pgm_test(const char *filename);
```
`pgm_test`函数用于测试文件(filename)是否为PGM文件，是则返回真（非0值）否则返回假(0).  
<br>

```C
macro: PGM_EMPTY
```
`PGM_EMPTY`宏用于代替`pgm_new`的实参列表，以新建一个空`Pgm`对象，或代替`pgm_set`从第二个开始的实参列表，以把指定`Pgm`对象设置为空`Pgm`对象·。  
<br>

### pgm扩展操作函数

```C
Pgm *pgm_set(Pgm *p5, uint16_t width, uint16_t height);
```
`pgm_set`函数用于重新设置指定Pgm对象，返回p5，当发生错误返回NULL。  
<br>

## 补充

### 空Pgm对象

可对空Pgm对象进行的操作的函数是受限的，其合法操作函数有: `pgm_free`, `img2pgm`, `pgm_info`, `pgm_set`以及其它用户自己设计的合理合法的操作函数。

### 用于防止Image重定义的宏
为了防止重新Image类型在部分语境下出现重复定义的情况，实现引进了两个宏 `__LIBPGM_INSIDE__` 和 `__LIBPGM_USE_IMAGE__`。

对于普通的libpgm用户来说，以上两个宏是无关紧要的，在源文件中直接包含pgm.h即可，无须（请勿）定义以上宏。  
对于libpgm的开发人员，如果libpgm中的源文件包含pgm.h，必须在 `#include "pgm.h"` 之前定义 `__LIBPGM_INSIDE__` 。如果该源文件还包含image.h，则还需要在 `#include "pgm.h"` 之前定义`__LIBPGM_USE_IMAGE__`。

## Know more
- [PGM Specification (P5)](./pgm-spec.md)