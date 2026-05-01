# Introduction of Portable Gray Map

pgm模块用于处理portable gray map (PGM)图像文件，其定义了用于存储PGM文件数据的`Pgm`结构。该模块主要负责PGM文件的读写及`Pgm`对象与`Image`对象的相互转换。  

portable gray map 图像文件是一种灰度图像文件，按照文件规范的定义其最大可支持65536阶灰阶，本实现现已符合该要求。当前pgm模块的实现符合了pgm (p5)规范的几乎所有要求。除了多图像pgm文件支持（没有打算支持）。  

注：虽说PGM实际上有两种类型P2和P5，但是本pgm模块仅用于处理P5类型的PGM文件。  

使用该模块需包含`pgm.h`，并编译相关C源文件或链接相关库。

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

```c
typedef uint16_t Pgm_px_def; // [0, 65535]
```

```c
typedef struct pgm {
	uint16_t width;  // width of pgm image
	uint16_t height; // height of pgm image
	uint16_t maxval; // max value of a pixel of pgm image
	Pgm_px_def *buf; // pixel buffer of pgm image
} Pgm;
```

### Pgm定义的别名和枚举
```c
typedef uint8_t Byte_t;

enum pgm_bit_depth_set {PGM_GRAY_4BIT  = 16		- 1, 
						PGM_GRAY_8BIT  = 256	- 1, 
						PGM_GRAY_10BIT = 1024	- 1, 
						PGM_GRAY_12BIT = 4096	- 1,
						PGM_GRAY_16BIT = 65536	- 1
};
```

## Pgm操作函数(宏)
```c
// pgm.h


/* basic operating functions of pgm */

/* New a pgm object */
Pgm *pgm_new(uint16_t width, uint16_t height, uint16_t maxval);
/* Free a pgm object */
int pgm_free(Pgm *p5);

/* Read a pgm file and create a Pgm object for it */
Pgm *pgm_read(const char *filename);
/* Write a pgm file */
int pgm_write(const char *filename);
/* Print the information of a Pgm object to specified file by the file pointer fileptr */
int Pgm_info(const Pgm *p5, void *fileptr);
/* Test a file is pgm file or not */
int pgm_valid(const char *filename);
/* Be used to new or re-set pgm object empty */
#define PGM_EMPTY 0, 0, PGM_GRAY_8BIT

/* extended operating functions of pgm */

/* Re-set a pgm object */
Pgm *pgm_set(Pgm *p5, uint16_t width, uint16_t height, uint16_t maxval);

/* Decode a pgm image */
int pgm_decode(Pgm_px_def *pxbuf, const Byte_t *iobuf, size_t cnt, uint16_t maxval);
/* Encode a pgm image */
int pgm_encode(Byte_t *iobuf, const Pgm_px_def *pxbuf, size_t cnt, uint16_t maxval);
```

## Pgm操作函数详述

### pgm基本操作函数

```c
Pgm *pgm_new(uint16_t width, uint16_t height, uint16_t maxval);
```
`pgm_new`函数用于通过参数`width`(宽度)、`height`(长度)和`maxval`最大灰度值创建对应规格的Pgm对象，返回指向该对象的指针，如果出错则返回NULL。新建的`Pgm`对象的画布颜色为黑色。  
<br>

```c
int pgm_free(Pgm *p5);
```
`pgm_free`函数用于释放`Pgm`对象，正常返回0异常返回非0值。  
<br>

```c
Pgm *pgm_read(const char *filename);
```
`pgm_read`函数用于读取PGM图像文件并为其创建Pgm对象，返回指向该对象的指针，当发生错误时返回NULL。  
<br>

```c
int pgm_write(const Pgm *p5, const char *filename);
```
`pgm_write`函数用于将`Pgm`对象的内容写到PGM图像文件中，正常返回0异常返回非0值。  
<br>

```c
int pgm_info(const Pgm *p5, void *fileptr);
```
`pgm_info`函数用于将显示`Pgm`对象的信息，正常返回0异常返回非0值。
注：`fileptr`实际是一个文件指针(`FILE *`)。  
<br>

```c
int pgm_valid(const char *filename);
```
`pgm_valid`函数用于测试文件(filename)是否为有效的PGM文件，是则返回真（非0值）否则返回假(0).  
<br>

```
macro: PGM_EMPTY
```
`PGM_EMPTY`宏用于代替`pgm_new`的实参列表，以新建一个空白`Pgm`对象，或代替`pgm_set`从第二个开始的实参列表，以把指定`Pgm`对象设置为空白`Pgm`对象·。  
<br>

### pgm扩展操作函数

```
Pgm *pgm_set(Pgm *p5, uint16_t width, uint16_t height);
```
`pgm_set`函数用于重新设置指定Pgm对象，返回`p5`，当发生错误返回NULL。  

如果图像宽度或高度发生了改变，则画面内容是由实现定义的。换言之，不要对此时的画面内容作任何假设。  
<br>

```c
int pgm_decode(Pgm_px_def *pxbuf, const Byte_t *iobuf, size_t cnt, uint16_t maxval);
```
`pgm_decode`解码pgm图像，出错时返回非零值。`cnt`为需处理的像素数量。  
<br>

```c
int pgm_encode(Byte_t *iobuf, const Pgm_px_def *pxbuf, size_t cnt, uint16_t maxval);
```
`pgm_endcode`编码pgm图像，出错时返回非零值。`cnt`为需处理的像素数量。  
<br>

```c
enum pgm_bit_depth_set;
```
`pgm_bit_depth_set`中的各枚举为`maxval`提供了几个常用的预设，可将该枚举中的枚举项作为`pgm_new`和`pgm_set`的`maxval`的实参使用。  

## 补充

### 65536阶灰阶的pgm像素
pgm结构的像素表示使用65536阶灰阶表示，如需将取值范围为$[0, 1]$的灰阶表示转换为65536阶灰阶表示可使用公式$y=65535x$，反向转换的公式由此易得。  

### 空白Pgm对象

可对空Pgm对象进行的操作的函数是受限的，其合法操作函数有: `pgm_free`, `pgm_info`, `pgm_set`以及其它用户自己设计的合理合法的操作函数。  

## Know more
- [PGM Specification (P5)](pgm-spec.md)