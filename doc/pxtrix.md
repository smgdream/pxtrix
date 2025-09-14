# Pxtrix

pxtrix(像素矩阵)是一个图像处理生成库，其架构如下
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
其由核心模块image和各种与实际图像格式相关的附加模块组成。一个模块主要构成为：一个与该模块存在的图像文件相关的图像类型、该类型的操作函数、有关的常量。  

pxtrix的核心是image模块以及该模块中的Image图像交换类型，该类型与具体图像格式无关，Image被用于处理和渲染图像。Render用于操作图像内容或绘制图形。附加模块负责具体图像格式的IO以及图像编解码。pic2img用于各图像类型和Image图像类型的相互转换。

NOTE0: 用户在正常情况下操作的是指向图像类型的指针，而不是图像类型结构，无论是Image还是附加模块定义的图像数据类型。
NOTE1: For native speaker of English you can see the comment in each header file.

## Know more

- [Image Intro](image.md)
- [Render library](render/render.md)
- [Color Look Up Table](render/lut.md)
- [Tiny Math Library](tmath.md)
- [Utility](util.md)
- [Example](examples/example.md)
- [Pic to Image](pic2img.md)
- [Introduction of model developement](dev/dev-intro.md)