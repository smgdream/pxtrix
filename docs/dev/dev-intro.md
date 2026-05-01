# Introduction of Pxtrix Developement
本文档用于说明Pxtrix项目的结构和内容，适用于Pxtrix的开发人员。

## Pxtrix目录结构介绍

### *.c（main.c test.c除外）
Pxtrix的核心组件的c文件

### include/*.h
Pxtrix核心组件的头文件。(util.h和perf.h除外)

### luts
色彩映射表（Color lookup table）

### util
实用工具源程序。其中包括：性能分析工具`perf.c`其对应的头文件为`perf.h`。以及一般的实用工具`util.c`对应头文件为`util.h`，`util.h`包含了`perf.h`。  

### addmods
i.e. addition modules. 读写编解码实际图像文件的“附加模块”存放在此目录中。  

| 目录名 | 对应的图像格式 |
| :- | - |
| libbmp | Microsoft Bitmap |
| libqoi | Quite OK Image |
| libpgm | Netpbm grayscale image |
| libfb | Linux framebuffer data/device |
| libca | Character Art (output only) |

各个附加模块之间以及和Pic2img模块和Pxtrix的核心模块之间不存在依赖关系，即各附加模块都是松耦合的这也就是“附加”二字的由来。  

各附加模块都各自包含一个makefile配置用于独立编译该模块。  

### lib2img
pic2img模块，其中的各个c源文件对应了不同实际图像数据类型和Image数据类型相互转换的函数。此目录还包含一个makefile文件。  

### tests
pxtrix的测试用例，主要供pxtrix的开发人员使用。  
注：tests不属于pxtrix库的组成部分。

## Pxtrix各组件的依赖关系
```
+---------+        +---------+       +-------------+      +--------+
|         |        |         |       |             |      |        |
|  LUTs   |        |  util   |       |             +----->| libbmp |
|         |        |         |       |             |      |        |
+----+----+        +---------+       |             |      +--------+
     |                               |             |      +--------+
     |                               |             |      |        |
     v                               |             +----->| libqoi |
+---------+        +---------+       |             |      |        |
|         |        |         |       |             |      +--------+
|         |        |         |       |             |      +--------+
|  Pixel  +------->|  Image  |<------+             |      |        |
|         |        |         |       |   pic2img   +----->| libpgm |
|         |        |         |       |             |      |        |
+----+----+        +---------+       |             |      +--------+
     |                  ^            |             |      +--------+
     |                  |            |             |      |        |
     v                  |            |             +----->| libfb  |
+---------+        +----+----+       |             |      |        |
|         |        |         |       |             |      +--------+
|         |        |         |       |             |      +--------+
|  tmath  |<-------+  Render |       |             |      |        |
|         |        |         |       |             +----->| libca  |
|         |        |         |       |             |      |        |
+---------+        +---------+       +-------------+      +--------+
```
说明：如下图，表示A是一个独立的没有依赖的组件，而B组件依赖于A组件。
```
+-----+      +-----+
|  B  +----->|  A  |
+-----+      +-----+
```

### Pxtrix各组件简介
| 常用组件名 | 说明 |
| - | - |
| Tiny Math | 轻量数学库，定义了一些数学常量和数学函数 |
| Pixel | 像素组件，定义了多种像素数据结构和像素处理函数 |
| Image | 定义了一种通用的与实际文件无关的图像数据结构`Image`及处理该结构的函数 |
| Render | “渲染器”组件，用于处理图像和绘制图像 |
| LTUs | 1D色彩查找表，将指定范围内的一个值映射到一个特定的颜色 |
| libbmp | 基本Microsoft Bitmap图像读写编解码库 |
| libqoi | Quite OK Image图像读写编解码库 |
| libpgm | Netpbm grayscale image (P5) 图像读写编解码库 |
| libfb | Linux framebuffer 设备/帧数据读写编解码库 |
| libca | Character Art（字符画）编码输出库 |
| pic2img | 用于将`Image`结构的数据和与实际图像文件关联的数据结构的数据的相互转换 |
| Utility | 包含了一些实用工具函数 |
| perf | 亚微秒级性能分析工具（亦为Utility的子模块） |

## 其他资料

附加模块开发指引见：[Introduction of Model Developement](mods-devel-intro.md)  
Tests组件的简要介绍见：[Introduction of tests](tests.md)  
更多资料可见[Pxtrix主文档](../pxtrix.md)及主文档的Know More。  