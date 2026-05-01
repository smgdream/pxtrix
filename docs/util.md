# Utility
Utility是一套实用工具集，其提供了一些实用函数用于辅助pxtrix的使用与调试。  

### 快速参考
```c
#include "util.h"

/* Sleeping in milliseconds */
int msleep(uint32_t ms);
/* defined a special literal */
#define obj_literal(TYPE, ...)
/* Write the byte sequence to temporary file */
FILE *byteseq2tmpfile(const uint8_t *seq, size_t size);
```

### 详细说明
```c
int msleep(uint32_t ms);
```
`msleep`用于实现毫秒级睡眠。  
<br>

```c
#define obj_literal(TYPE, ...)
```
`obj_literal`带参宏，用于定义一个特殊的字面值，用于创建聚合对象字面值（我也不知道具体要如何解释 :P ，自己看util.h中的定义琢磨）  

```c
FILE *byteseq2tmpfile(const uint8_t *seq, size_t size);
```
`byteseq2tmpfile`将`seq`指向的字节数组写入到一个临时文件中，并返回该文件指针，如果出错则返回NULL。返回的文件指针的文件位置将指向文件的开始。  

注1：虽然该流在程序结束时会自动关闭，但仍建议在不再使用该流后通过`fclose`显式关闭该流。  

注2：如果在Windows 7及更新系统中在C盘里运行使用了该函数的程序可能会出现Permission denied问题。解决方法为：在其他盘中执行程序或以管理员身份运行。  

## pref
pref为util的子组件，其用于获取代码运行耗时。

### 快速参考
```c
#include "pref.h"

/* unit of time: us */

/* Start the timer */
void timer_start(void);

/* Reset the timer */
void timer_reset(void);

/* Different of the time between prev call and this call */
uint64_t time_step(void);

/* The time total used after the timer start or reset */
uint64_t time_all(void);
```

### pref组件的函数说明

```c
void timer_start(void);
```
`time_start`函数用于启动计时器。

```c
void timer_reset(void);
```
`timer_reset`函数将重置计时器。

```c
uint64_t time_step(void);
```
`time_step`函数将返回上一次计时执行到该次执行间隔的时间(微秒)。(`time_start`, `time_reset`, `time_step` 为计时执行操作)

```c
uint64_t time_all(void);
```
`time_all`函数返回从上次置时到当前经过的时间(微秒)。(`time_start`, `time_reset` 为置时操作)

