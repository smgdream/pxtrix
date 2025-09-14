# Introduction of Framebuffer Device

fbdev模块用于对framebuffer文件/设备进行处理。其定义了一个Fbdev图像类型用于存储framebuffer文件/设备的信息和数据。还定义了一系列用于操作framebuffer文件/设备和Fbdev对象的函数。    

## 使用说明
使用时需包含`fbdev.h`并编译相关C源文件或链接相应的库，用户在使用时应通过操作函数对`Fbdev`对象进行操作，不建议直接访问`Fbdev`对象的内部成员。    

## 像素排列

```
0             x
  +---------->
  |
  |    fb
  |
y v
```

## 结构定义

```c
// fbdev.h

typedef struct fb_px_def {
	uint8_t b; // blue
	uint8_t g; // green
	uint8_t r; // red
	uint8_t a; // alpha
} Fb_px_def;

typedef struct fbdev {
	uint32_t xres; // x resolution
	uint32_t yres; // y resolution
	Fb_px_def *buf;// pixels buffer
} Fbdev;
```

## 枚举定义

```c
// fbdev.h

enum fbdev_flags { FB_FILE = 0, FB_DEVICES = 1 };
```

## Fbdev操作函数

```c
// fbdev.h

/* New a Fbdev object */
Fbdev *fb_new(uint32_t xres, uint32_t yres);
/* Free the Fbdev object */
int fb_free(Fbdev *fbd);
/* Read a FB file/device and create a Fbdev object for it */
Fbdev *fb_read(const char *filename);
/* Write the Fbdev object to a file */
int fb_write(const Fbdev *fbd, const char *filename);

/* Test a fb file is valid or not */
int fb_valid(const char *filename);
/* Print the information of a Fbdev object */
int fb_info(const Fbdev *fbd, void *fileptr);
#define FB_EMPTY 0, 0

/* Reconfigure a Fbdev object */
Fbdev *fb_set(Fbdev *fbd, uint32_t xres, uint32_t yres, uint8_t dev_flag);
/* Get the pixel at (x, y) in specifical Fbdev object */
Fb_px_def *fb_px(const Fbdev *fbd, uint32_t x, uint32_t y)
```

### 详细说明

```c
Fbdev *fb_new(uint32_t xres, uint32_t yres, uint8_t dev_flag);
```
`fb_new`用于新建一个`Fbdev`对象，`xres`用于指定图像x方向的分辨率（即图像宽度），`yres`用于指定图像y方向的分辨率（即图像高度），`dev_flag`用于指定framebuffer对象类型,对应可用值为`FB_FILE`或`FB_DEVICE`。正常返回指向`Fbdev`对象的指针，出错则返回`NULL`.  
<br>

```c
int fb_free(Fbdev *fbd);
```
`fb_free`释放一个Fbdev对象。正常返回0，异常返回非0值。  
<br>

```c
Fbdev *fb_read(const char *filename);
```
`fb_read`读取一个framebuffer文件/设备，并为其创建一个`Fbdev`对象。返回指向改对象的指针，出错则返回`NULL`.  
<br>

```c
int fb_write(const Fbdev *fbd, const char *filename);
```
`fb_write`将一个`Fbdev`对象写到framebuffer文件/设备中。正常返回0，异常返回非0值。  
<br>

```c
int fb_valid(const char *filename);
```
`fb_valid`测试`filename`是否为有效的framebuffer文件/设备。是则返回真（非0）否则返回假（0）。
<br>

```c
int fb_info(const Fbdev* fbd, const char *fileptr);
```
`fb_info`将指定`Fbdev`对象的信息输出到文件流`fileptr`中。
<br>

```c
#define FB_EMPTY 0, 0
```
`FB_EMPTY`宏用于作为`fb_new`或`fb_set`的参数以新建空白`Fbdev`对象。  
<br>

```c
Fbdev *fb_set(Fbdev *fbd, uint32_t xres, uint32_t yres);
```
`fb_set`重新设置指定的`Fbdev`对象，该函数可能会重新分配buffer。返回`fbd`，出错则返回`NULL`.  
<br>

```c
Fb_px_def *fb_px(const Fbdev *fbd, uint32_t x, uint32_t y);
```
`fb_px`获取在指定`Fbdev`中坐标为(x，y)的像素。返回指向对应像素点的指针，出错时返回`NULL`.  
<br>

## 补充

### 空白Fbdev对象
可对空白`Fbdev`对象执行的操作是受限的，仅允许以下操作函数: `fb_free`, `img2fb`, `fb_info`, `fb_set`以及其他由用户编写的合理的操作函数。  

## Know more
- [The Frame Buffer Device](fb-device.md)
- [The Frame Buffer Device API](fbdev-api.md)