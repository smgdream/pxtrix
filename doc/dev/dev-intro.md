# Introduction of Model Developement
version: 0.2

附加模块正常情况下与实际图像文件相关联。模块应包含一个图像数据结构类型，用于存储图像的属性和像素数据。模块还要包含用于操作图像数据类型的操作函数和有关的宏。

## 用户接口说明

提供给用户接口有三个部分: 指向图像数据类型的指针，用于操作图像数据类型的函数，有关的宏与枚举。

### 指向图像数据类型的指针

按照附加模块的设计理念，模块的设计类似于C标准库的FILE。提供给用户的对象是一个指向图像数据结构类型的指针而不是具体的图像数据结构，但不建议将图像数据类型封装为ADT，而应像FILE一样允许用户进行特殊操作(但在文档中应说明: 不建议用户去操作指针指向的图像数据结构内部的具体内容)。当然，在正常情况下我们更希望用户将图像数据类型当作ADT来使用。  

### 操作图像数据的函数

操作图像图像数据的函数如果要接受一个图像数据类型，则对应参数应该为图像数据类型的指针，而如果一个函数要返回图像数据类型则返回的应该是指向图像数据类型的指针。  
本开发指南定义了一些提供给用户的基本操作函数集，附加模块中必须拥有这些函数，当然也可以根据实际情况提供一些拓展的操作函数。但实际上模块中可能还包含一些用于其它用途的函数，这些函数可以根据实际情况来确定是否向用户开放。  

### 有关的宏与枚举
主要为一些常量命名方便用户在需要的时候使用。  

## 具体图像类型的基本操作函数
具体图像类型是指某种与实际图像文件对应的数据的数据存储结构。为了方便说明，我们假设存在一种全称为"picture"的图像文件，模块中与其相关联的图像数据存储结构的类型别名被定义为`Pictu`，对于与其相关的函数和宏我们将其用于标示的名称定义为*pic*。
基本操作函数是各附加模块所必须拥有的对用户开放的函数接口。`Pictu`对象应存在与图像像素缓冲区关联的成员。  

### 基本操作函数的格式规范及其实现说明

```c
Pictu *pic_new(args);
```
提供参数新建一个`Pictu`对象并申请像素缓冲区与其关联。返回指向该`Pictu`对象的指针或NULL如果出错。像素缓冲区应全0初始化。该函数用于画布空白的对象。  
<br>

```c
int pic_free(Pictu *p);
```
释放`p`指向的对象。正常返回0，异常返回非0值。  
<br>

```c
Pictu *pic_read(const char *filename);
```
读取名为filename的picture文件并为其创建一个合适的`Pictu`对象，将文件数据存入其中。返回指向该`Pictu`对象的指针，如果出错则返回NULL。  
<br>

```c
int pic_write(const Pictu *p, const char *filename);
```
将`p`指向对象的内容写入到filename中。正常返回0，异常返回非0值。  
<br>

```c
Image *pic2img(const Pictu *src, Image *dest);
```
将src指向的`Pictu`对象转换为`Image`对象存入`dest`中，如果`dest`的规格不合适，则重新设置`dest`指向的对象的规格至刚刚好。正常返回`dest`，异常返回NULL。  
<br>

```c
Pictu *img2pic(const Image *src, Pictu *dest);
```
将src指向的`Image`对象转换为`Pictu`对象存入`dest`中，如果`dest`的规格不合适，则重新设置`dest`指向的对象的规格至刚刚好。正常返回`dest`，异常返回NULL。  
<br>

```c
int pic_info(const Pictu *p, void *fileptr); // opt
```
(可选)将`p`指向的`Pictu`对象中的图像信息输出的文件流`fileptr`(`fileptr`虽声明为`void *`，但其实际上为文件指针，这是为了避免在头文件中多包含一个stdio.h)。正常返回0，异常返回非0值。  
<br>

```c
int pic_test(const char *filename);
```
测试`filename`是否为有效的Picture类型的图像文件。“是”则返回真（非0值），“否”则返回假（数值0）。  
<br>

```c
macro: PIC_EMPTY
```
一个宏，可用于作为`pic_new`的参数，用来创建一个空白`Pictu`对象。如:   
```c
Pictu *ept_picture = pic_new(PIC_EMPTY);
```
<br>

## 补充

### 防止Image重定义
在部分语境下可能出现警告Image类型重定义的情况。如果要避免触发该警告可以自行思考设计实现。下文给出了一个供参考的实现。
```C
#ifdef __LIBPIC_INSIDE__
	#if !defined(__IMAGE_TYPE__) && !defined(__LIBPIC_USE_IMAGE__)
		typedef struct image Image;
	#endif
	#ifdef __LIBPIC_USE_IMAGE__
		#include "image.h"
	#endif
#else
	#include "image.h"
#endif
```
实现说明：  
为了防止重新Image类型在部分语境下出现重复定义的情况，实现引进了两个宏 `__LIBPIC_INSIDE__` 和 `__LIBPIC_USE_IMAGE__`。

对于普通的libpic用户来说，以上两个宏是无关紧要的，在源文件中直接包含pictuer.h即可，无须（请勿）定义以上宏。  
对于libpic组件的开发人员，如果libpic组件中的源文件包含pictuer.h，必须在 `#include "pictuer.h"` 之前定义 `__LIBPIC_INSIDE__` 。如果该源文件还包含image.h，则还需要在 `#include "picture.h"` 之前定义`__LIBPIC_USE_IMAGE__`。

## Example
- [qoi model](../qoi/qoi-intro.md)