# Pxtrix

pxtrix(像素矩阵)是一个图像处理生成库，其架构如下
```
[bmp]<----+   +----->[pgm]
          |   |
		  v   v
         [image] <==> {Render}
          ^   ^
          |   |
[qoi]<----+   +----->[fbdev]
```
其由核心模块image和各种与实际图像格式相关的附加模块组成。一个模块主要构成为：一个与该模块存在的图像文件相关的图像类型、该类型的操作函数、有关的常量。  

pxtrix的核心是image模块以及该模块中的Image图像交换类型，该类型与具体图像格式无关，Image被用于处理和渲染图像。附加模块负责具体图像格式的IO、编解码以及和Image图像类型的相互转换。

NOTE: 用户在正常情况下操作的是指向图像类型的指针，而不是图像类型结构。

## Know more

- [Image Intro](./image.md)
- [Render library](./render/render.md)
- [Color Look Up Table](./render/lut.md)
- [Tiny Math Library](./tmath.md)
- [Utility](./util.md)
- [Example](./examples/example.md)