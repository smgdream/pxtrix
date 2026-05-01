# Color Lookup Table

render渲染组件提供了多个 1D LUT ((color) look up table) 色彩查找表。  

## Usage

包含`luts/luts.h`后即可使用所有的LUT，也可以包含特定的`luts/lut_LUTNAME.h`以明确的使用特定LUT。随后在需要使用特定LUT时使用`lut_LUTNAME`函数即可。各LUT函数的函数原型均如下所示：  
```c
Px_rgba lut_LUTNAME(float val);
```
函数的定义域为[0, 1]<sup>注1</sup>，返回具有相应颜色的`Px_rgba`对象。通过类似`lut_LUTMAME(x).r`的形式可以获得函数返回像素的特定通道分量。  

注1：传递给`val`不在定义域内的实参值是未定义行为。  

各LUT的名称、色彩、LUT函数名和文件名见下表。  
| Name		| Color			| LUT function name	| Header name	|
| --------- | ------------- | ----------------- | ------------- |
| hsv		| black->white	| lut_hsv			| lut_hsv.h		|
| hot		| colorful		| lut_hot			| lut_hot.h		|
| magma		| blue->pink	| lut_magma			| lut_magma.h	|
| inferon	| blue->pink	| lut_inferon		| lut_inferon.h	|