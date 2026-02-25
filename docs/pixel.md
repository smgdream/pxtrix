# Pixel
Pixel模块定义了数种像素格式以及相关函数和宏。  

## 像素格式

### 32bit RGBA
8位位深的RGBA色彩模型像素格式（即RGBA8888），各通道取值范围见下面的代码注释。  
```c
typedef struct px_rgba_32bit {
	uint8_t r; // red	[0, 255]
	uint8_t g; // green	[0, 255]
	uint8_t b; // blue	[0, 255]
	uint8_t a; // alpha	[0, 255]
} Px_rgba_32bit, Px_rgba32_t, Px_rgba, Px_def;
```

### HSVA
HSVA色彩模型像素格式，各通道取值范围见下方代码的注释。  
```c
typedef struct px_hsva_float {
	float h; // Hue			[0, 360]
	float s; // Saturation	[0, 1]
	float v; // Value		[0, 1]
	float a; // Alpha		[0, 1]
} Px_hsva_float, Px_hsva_f, Px_hsva;
```

## 函数和宏
```c
/* Create a value of RGBA pixel by an eight numbers hexadecimat sequence. */
Px_rgba rgba_hex(uint32_t hex_val);
/* Mapping RGBA pixel to HSVA pixel */
Px_hsva rgba2hsva(Px_def rgba);
/* Mapping HSVA pixel to RGBA pixel */
Px_def hsva2rgba(Px_hsva hsva);
```

## 详细说明

```c
Px_rgba rgba_hex(uint32_t hex_val);
```
`rgba_hex`将一个由8位数的十六进制数表示的RGBA值转换为相应的`Px_rgba`值。通常其有效输入形式为`0xRRGGBBAA`，例如：`0x39C5BBFF`  
<br>

```c
Px_hsva rgba2hsva(Px_rgba rgba);
```
`rgba2hsva`将一个`Px_rgba`值映射成`Px_hsva`像素值。  
<br>

```c
Px_def hsva2rgba(Px_hsva hsva);
```
`hsva2rgba`将一个`Px_hsva`值映射成`Px_rgba`像素值。经过完整测试，使用当前算法将RGBA像素转换为HSVA再转换回RGBA的颜色微偏<sup>注1</sup>率为56.21%  

注1：颜色微偏指像素颜色经过来回映射后映射成与原始颜色相邻（肉眼通常难以区分两种颜色）颜色的情况。
<br>

