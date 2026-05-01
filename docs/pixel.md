# Pixel
Pixel模块定义了数种像素格式以及相关函数和宏。  

## 像素格式

`Px_def`表示默认的像素格式，`Px_rgba`表示默认的RGBA像素格式，`Px_hsva`表示默认的HSVA像素格式。

### RGBA Float32
RGBA色彩模型32位浮点通道的像素结构，亦是`Px_rgba`和`Px_def`类型别名当前对应的像素结构。各通道取值范围见下方代码注释。  
```c
/* pixel.h */

typedef struct px_rgba_float32 {
	float r; // red		[0, 1]
	float g; // green	[0, 1]
	float b; // blue	[0, 1]
	float a; // alpha	[0, 1]
} Px_rgba_float32, Px_rgba_f32, Px_rgba, Px_def;
```

### HSVA
HSVA色彩模型像素结构，各通道取值范围见下方代码的注释。  
```c
/* pixel.h */

typedef struct px_hsva_float {
	float h; // Hue			[0, 360]
	float s; // Saturation	[0, 1]
	float v; // Value		[0, 1]
	float a; // Alpha		[0, 1]
} Px_hsva_float, Px_hsva_f, Px_hsva;
```
NOTE1: 此HSVA像素的Hue通道有效取值设置为闭合区间$[0, 360]$，而非不包含360的半闭合区间。在通过自定义的算法对HSVA像素进行计算时应当注意H = 360的情况。  

### RGBA 32bit
8位位深的RGBA色彩模型像素结构（即RGBA8888），各通道取值范围见下面的代码注释。  
```c
/* pixel.h */

typedef struct px_rgba_32bit {
	uint8_t r; // red	[0, 255]
	uint8_t g; // green	[0, 255]
	uint8_t b; // blue	[0, 255]
	uint8_t a; // alpha	[0, 255]
} Px_rgba_32bit, Px_rgba32_t, Px_rgba_8888;
```

## 函数和宏
```c
/* pixel.h */

/* Create a value of RGBA pixel by an eight numbers hexadecimat sequence */
Px_rgba rgba_hex(uint32_t hex_val);
/* Create a value of RGBA pixel by 4 channel value */
Px_rgba rgba_4val(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/* Alpha blending */
Px_rgba alpha_blend(Px_rgba dest, Px_rgba src);

/* Pixel equal. Returns 1 if equal 0 or not  */
int px_eq(Px_def px1, Px_def px2);
/* Pixel almost equal. Returns 1 if almost equal 0 or not  */
int px_aeq(Px_def px1, Px_def px2);

/* Mapping RGBA pixel to HSVA pixel */
Px_hsva rgba2hsva(Px_rgba rgba);
/* Mapping HSVA pixel to RGBA pixel */
Px_rgba hsva2rgba(Px_hsva hsva);
```

## 详细说明

```c
Px_rgba rgba_hex(uint32_t hex_val);
```
`rgba_hex`将一个由8位十六进制数表示的RGBA值转换为相应的`Px_rgba`值。通常其有效输入形式为`0xRRGGBBAA`，例如：`0x39C5BBFF`  
<br>

```c
Px_rgba rgba_4val(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
```
`rgba_4val`通过分别表示RGBA的4个取值范围为`[0, 255]`的值创建一个相应颜色的`Px_rgba`值。
<br>

```c
Px_rgba alpha_blend(Px_rgba dest, Px_rgba src);
```
`alpha_blend`用于alpha混合，其将两个半透明的像素正确混合，返回src叠在dest上的结果。当前实现在计算时才将RGB通道和Alpha相乘。  
<br>

```c
int px_eq(Px_def px1, Px_def px2);
```
`px_eq`用于比较两个像素是否相等，相等则返回1，否则返回0。  
<br>

```c
int px_aeq(Px_def px1, Px_def px2);
```
`px_aeq`用于比较两个像素是否相似，相似返回1，否则返回0。当前实现保证在65536阶通道表示的RGBA超立方体中相邻颜色必然是不相似的。  
<br>

```c
Px_hsva rgba2hsva(Px_rgba rgba);
```
`rgba2hsva`将一个`Px_rgba`值映射成`Px_hsva`像素值。  
<br>

```c
Px_hsva hsva2rgba(Px_hsva hsva);
```
`hsva2rgba`将一个`Px_hsva`值映射成`Px_rgba`像素值。经过完整测试，使用当前算法将RGBA像素转换为HSVA再转换回RGBA的颜色平均偏移值<sup>注1</sup>约为0.00000254%  

注1：颜色偏移值指像素颜色经过来回映射后映射值与原始颜色的色差。RGB立方体中，互补色的相互之间的色差为100%
<br>

