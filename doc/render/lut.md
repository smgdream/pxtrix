# Color Lookup Table

render渲染组件提供了多个 1D LUT ((color) look up table) 色彩查找表。

## Usage

通过`include`预处理指令为当前c文件加载LUT，LUT均存放在luts文件夹中。  
```c
// example
#include "luts/clr_lut_magma.lut"
```
其作用为定义了一个长度为256的类型为Px_def的静态数组 `clr_ltu_*` (e.g. `clr_lut_magma`)  

该数组可通过类似 `clr_lut_*[index].?` 的形式单独使用其 R、G、B、A 通道的分量 (e.g. `clr_lut_magma[i].r`)，也可以通过类似 `clr_lut_*[index]` 的形式使用整个像素 (e.g. `clr_lut_magma[i]`)，Example: Available at `examples/mandb_color.c`.  

各LUT的名称、色彩、文件名见下表。
| name		| color			| file name				|
| --------- | ------------- | --------------------- |
| hsv		| black->white	| clr_lut_hsv.lut		|
| hot		| colorful		| clr_lut_hot.lut		|
| magma		| blue->pink	| clr_lut_magma.lut		|
| inferon	| blue->pink	| clr_lut_inferon.lut	|