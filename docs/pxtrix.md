# Pxtrix

Pxtrix(像素矩阵)是一个图像处理生成库，其架构如下：  
```
+-----+  +-----+  +-------+  +-----+
| bmp |  | qoi |  | fbdev |  | pgm |
+-----+  +-----+  +-------+  +-----+
   ^        ^         ^         ^
   |        |         |         |
   v        v         v         v
 +---------------------------------+
 |             pic2img             |
 +---------------------------------+
         ^          
	     |           +--------+
         v           |        |
     +-------+       | Render |
     | Image | <===> |        |
     +-------+       +--------+
```
其由核心模块image和各种与实际图像格式相关的附加模块组成。一个附加模块主要构成为：一个与该模块存在的图像文件相关的图像类型、该类型的操作函数、有关的常量。  

pxtrix的核心是Image模块以及该模块中的Image图像交换类型，该类型与具体图像格式无关，Image被用于处理和渲染图像。Image模块还包含一个子模块Pixel, Pixel模块定义了数种常用像素格式及一些相关的函数和宏。Render用于操作图像内容或绘制图形。附加模块负责具体图像格式的IO及编解码。pic2img模块用于各图像类型和Image图像类型的相互转换。  

- NOTE0: For native speaker of English you can see the comment in each header file. Also you can reading the documents by translator.   
  
- NOTE1: 对于Image结构来说，用户可以只读访问Image图像数据结构除像素缓冲区之外的成员，但不应修改。对像素的操作应当通过`img_px`函数进行。  

- NOTE2: 对于附加模块定义的图像数据类型来说，用户在正常情况下操作的是指向图像类型的指针，而不是图像类型结构。用户应当把附加模块定义的图像数据类型当作抽象数据类型(ADT)来使用。  

- NOTE3: 在编写调用Pxtrix库的代码时可能涉及有符号整型、无符号整型和浮点型之间的相互转换和混合计算，请务必注意类型转换的潜在陷阱。  

## 项目开发过程中AI辅助的说明
仅局限于：
- 极个别（英文）注释的行文和语法调整。  

## Know more

- [Image Intro](image.md)
- [Pixel](pixel.md)
- [Render library](render/render.md)
- [Color Look Up Table](render/luts.md)
- [Tiny Math Library](tmath.md)
- [Utility](util.md)
- [Additional Models](mods)
- [Pic to Image](pic2img.md)
- [Example](examples/example.md)
- [Introduction of Pxtrix developement](dev/dev-intro.md)