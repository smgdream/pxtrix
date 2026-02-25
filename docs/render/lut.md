# Color Lookup Table

render渲染组件提供了多个 1D LUT ((color) look up table) 色彩查找表。

## Usage

通过`include`预处理指令为当前c文件加载LUT，LUT均存放在luts文件夹中。  
```
// example
#include "luts/lut_magma.h"
```
其作用为定义了一个长度为256的类型为Px_def的静态数组 `clr_ltu_*` (e.g. `clr_lut_magma`)  

该数组可通过类似 `clr_lut_*[index].?` 的形式单独使用其 R、G、B、A 通道的分量 (e.g. `clr_lut_magma[i].r`)，也可以通过类似 `clr_lut_*[index]` 的形式使用整个像素 (e.g. `clr_lut_magma[i]`)，Example: Available at `examples/mandb_color.c`.  
Note: `*`表示对应的LUT Name

各LUT的名称、色彩、颜色映射数组名和文件名见下表。
| Name		| Color			| Lut array name	| Header name	|
| --------- | ------------- | ----------------- | ------------- |
| hsv		| black->white	| clr_lut_hsv		| lut_hsv.h		|
| hot		| colorful		| clr_lut_hot		| lut_hot.h		|
| magma		| blue->pink	| clr_lut_magma		| lut_magma.h	|
| inferon	| blue->pink	| clr_lut_inferon	| lut_inferon.h	|