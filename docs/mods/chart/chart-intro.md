# Introduction of Character Art

Chart模块用于将图像以字符画形式输出到终端，其定义了一个图像数据类型Chart，还定义了一系列用于操作Chart对象的操作函数。  

## 使用说明

使用Chart模块需包含`chart.h`头文件并编译相应C源文件或链接相应的库。正常情况下用户使用的是指向Chart的指针(i.e. `Chart *`)以及用于操作Chart对象的操作函数。不建议直接访问Chart对象的内部成员。  
## 像素排列

```
0             x
  +---------->
  |
  |  chart
  |
y v
```

## Chart结构定义

```c
// chart.h

typedef uint8_t Ca_px_def;  // [0, 255]

typedef struct chart {
	uint16_t width;	// width
	uint16_t height;// height
	uint16_t cols;	// units per line
	Ca_px_def *buf;	// pixels buffer
} Chart;
```

## Chart操作函数

```c
// chart.h

/* New a Chart object */
Chart *ca_new(uint16_t wid, uint16_t hgt, uint16_t cols);
/* Free a Chart object */
int ca_free(Chart *ca);
/* Write the Chart object to a specified stream  */
int ca_write(const Chart *ca, double wid_rate, void *fileptr);
/* Print the information of Chart object to a stream (fileptr) */
int ca_info(const Chart *ca, void *fileptr);

/* CA_EMPTY used to create or set an empty Chart object */
#define CA_EMPTY 0, 0, _CA_TERM_D4_WID

/* Reconfigure the specified Chart object */
Chart *ca_set(Chart *ca, uint16_t wid, uint16_t hgt, uint16_t cols);
/* Get the pixel at (x, y) in a Chart object */
Ca_px_def *ca_px(const Chart *ca, uint16_t x, uint16_t y);
```

### 基本操作详细说明

```c
Chart *ca_new(uint16_t wid, uint16_t hgt, uint16_t cols);
```
`ca_new`新建一个Chart对象，`wid`用于指定图像的宽度，`hgt`用于指定图像的高度，`cols`用于指定字符画每行的字符数。返回指向该对象的指针，出错则返回`NULL`。`Chart`对象的画布颜色为黑色。  
<br>

```c
int ca_free(Chart *ca);
```
`ca_free`释放指定的Chart对象。正常返回0，异常返回非0值。  
<br>

```c
int ca_write(const Chart *ca, double wid_rate, void *fileptr);
```
`ca_write`将`ca`以字符画的形式输出到文件流`fileptr`，参数`wid_rate`用于控制字符画宽度的缩放比例（正常: 1，其定义域为[1, 2]）。正常返回0，异常返回非0值。  
<br>

```c
int ca_info(const Chart *ca, void *fileptr);
```
`ca_info`将Chart对象的信息输出到文件流`fileptr`中。正常返回0，异常返回非0值。  
<br>

```
macro: CA_EMPTY
```
`CA_EMPTY`宏可用于代替`ca_new`函数的实参列表用以创建一个空白Chart对象, e.g. `ca_new(CA_EMPTY)`. 亦可用于代替`ca_set`函数的第一个参数之后的所有参数用以将指定Chart对象设置为空白Chart对象, e.g. `ca_set(cap, CA_EMPTY)`.  
<br>

### 扩展操作详细说明

```c
Chart *ca_set(Chart *ca, uint16_t wid, uint16_t hgt, uint16_t cols);
```
`ca_set`用于重新设置指定Chart对象。返回`ca`，出错则返回`NULL`.  

如果图像宽度或高度发生了改变，则画面内容是由实现定义的。换言之，不要对此时的画面内容作任何假设。  
<br>

```c
Ca_px_def *ca_px(const Chart *ca, uint16_t x, uint16_t y);
```
`ca_px`用于获得指定Chart对象中坐标为(x, y)的像素。返回指向对应像素的指针，如果发生错误则返回`NULL`.  
<br>

## 补充

### 空白Chart对象
可对空白Chart对象执行的操作是受限的，仅允许以下操作函数: `ca_free`, `ca_info`, `ca_set`以及其他由用户自行设计的合理操作函数。  
