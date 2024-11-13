# Other

## pref
pref组件用于获取代码运行耗时
```c
/* pref.h */
/* unit of time: us */

/* timer start */
void timer_start(void);

/* Reset the timer */
void timer_reset(void);

/* Different of the time between prev call and this call */
uint32_t time_step(void);

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
uint32_t time_step(void);
```
`time_step`函数将返回上一次计时执行到该次执行见隔的时间(微秒)。(time_start, time_reset, time_step 为计时执行操作)

```c
uint64_t time_all(void);
```
`time_all`函数返回从上次置时到当前经过的时间(微秒)。(time_start, time_reset 为置时操作)