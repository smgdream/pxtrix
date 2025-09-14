# Introduction of Portable Gray Map

pgm模块用于处理portable gray map (PGM)图像文件，其定义了用于存储PGM文件数据的`Pgm`结构。该模块主要负责PGM文件的读写及`Pgm`对象与`Image`对象的相互转换。  
portable gray map 图像文件是一种灰度图像文件，按照文件规范的定义其最大可支持65536阶灰阶，但本实现目前仅支持256阶灰阶。  
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
typedef struct pgm {
	uint16_t width;  // width of pgm image
	uint16_t height; // height of pgm image
	uint16_t maxval; // max value of a pixel of pgm image
	Pgm_px_def *buf; // pixel buffer of pgm image
} Pgm;
```

```c
typedef uint8_t Pgm_px_def;
```

## Pgm操作函数(宏)
```c
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
/* Print the information of a Pgm object to specified file by the file pointer fileptr */
int Pgm_info(const Pgm *p5, void *fileptr);
/* Test a file is pgm file or not */
int pgm_valid(const char *filename);
/* Be used to new or re-set pgm object empty */
#define PGM_EMPTY 0, 0

/* extended operating functions of pgm */

/* Re-set a pgm object */
Pgm *pgm_set(Pgm *p5, uint16_t width, uint16_t height);
```

## Pgm操作函数详述

### pgm基本操作函数

```c
Pgm *pgm_new(uint16_t width, uint16_t height);
```
`pgm_new`函数用于通过参数width(宽度)和height(长度)创建对应规格的Pgm对象，返回指向该对象的指针，但如果出错则返回NULL。  
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
`pgm_test`函数用于测试文件(filename)是否为有效的PGM文件，是则返回真（非0值）否则返回假(0).  
<br>

```
macro: PGM_EMPTY
```
`PGM_EMPTY`宏用于代替`pgm_new`的实参列表，以新建一个空`Pgm`对象，或代替`pgm_set`从第二个开始的实参列表，以把指定`Pgm`对象设置为空`Pgm`对象·。  
<br>

### pgm扩展操作函数

```
Pgm *pgm_set(Pgm *p5, uint16_t width, uint16_t height);
```
`pgm_set`函数用于重新设置指定Pgm对象，返回p5，当发生错误返回NULL。  
<br>

## 补充

### 空Pgm对象

可对空Pgm对象进行的操作的函数是受限的，其合法操作函数有: `pgm_free`, `img2pgm`, `pgm_info`, `pgm_set`以及其它用户自己设计的合理合法的操作函数。  
## Know more
- [PGM Specification (P5)](pgm-spec.md)