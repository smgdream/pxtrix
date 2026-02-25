# Example
| Filename						| Function									| Comment |
| ----------------------------- | ------------------------------------------| - |
| [bezier.c](bezier.c)			| draw a bezier curve						| - |
| [envelope.c](envelope.c)		| draw a finger about *cos(0.5x)sin(10x)*	| - |
| [lut_get.c](lut_get.c)		| get the rbg value of a 256xN 1D LUT image	| - |
| [mandb_color.c](mandb_color.c)| draw a colorful mandelbrot set			| Not support for tcc.<br> arg1: size (px); arg2: gamma;<br>e.g. a.out 2160 0.3 |
| [mdb_thread.c](mdb_thread.c)	| draw a mandelbrot set (multhread version)	| Not support for tcc.<br> arg1: size (px); arg2: gamma;<br>e.g. a.out 2160 0.3 |
| [pgm_test.c](pgm_test.c)		| convert a pgm image to bmp image			| - |
| [test.c](test.c)				| crop image								| - |
| [mdb_intact](mdb_intact/) | mandelbrot set interactive viewer | arg1: width of screen (px); arg2: height of screen (px); arg3: gamma;<br>e.g. a.out 3840 2160 0.3<br>This example must be run in Linux virtual terminal enviroment and make sure you can write framebuffer device.<br><br> Control:<br> up: w<br> down: s<br> left: a<br> right:d<br> zoom in: =<br> zoom out: -<br> incerasing the number of iterations: ]<br> decreasing the number of iterations: [<br> default iterating times: 256 |
<!-- | 				|  | - | -->
## Usage
copy a example to the root of pxtrix and rename it as main.c (also you can edit the makefile instead of rename).
