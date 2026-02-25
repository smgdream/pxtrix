# Utility

```
/* defined a special literal */
#define obj_literal(TYPE, ...)
```

### 详细说明
```
#define obj_literal(TYPE, ...)
```
`obj_literal`带参宏，用于定义一个特殊的字面值，用于创建聚合对象字面值（我也不知道具体要如何解释 :P ，自己看util.h中的定义琢磨）  


## pref
pref组件用于获取代码运行耗时
pref model used to get the time take
```
/* pref.h */
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

```
void timer_start(void);
```
`time_start`函数用于启动计时器。

```
void timer_reset(void);
```
`timer_reset`函数将重置计时器。

```
uint64_t time_step(void);
```
`time_step`函数将返回上一次计时执行到该次执行见隔的时间(微秒)。(time_start, time_reset, time_step 为计时执行操作)

```
uint64_t time_all(void);
```
`time_all`函数返回从上次置时到当前经过的时间(微秒)。(time_start, time_reset 为置时操作)

