# Example
| Filename						| Function									| Comment |
| ----------------------------- | ------------------------------------------| - |
| [bezier.c](bezier.c)			| draw a bezier curve						| - |
| [envelope.c](envelope.c)		| draw a finger about *cos(0.5x)sin(10x)*	| - |
| [lut_get.c](lut_get.c)		| get the rbg value of a 256xN 1D LUT image	| - |
| [mandb_color.c](mandb_color.c)| draw a colorful mandelbrot set			| arg[1]: size; arg[2]: gamut; Not support for tcc |
| [mdb_thread.c](mdb_thread.c)	| draw a mandelbrot set (multhread version)	| arg[1]: size; arg[2]: gamut; Not support for tcc |
| [pgm_test.c](pgm_test.c)		| convert a pgm image to bmp image			| - |
| [test.c](test.c)				| crop image								| - |
<!-- | 				|  | - | -->
## Usage
copy a example to the root of pxtrix and rename it as main.c (also you can edit the makefile instead of rename).
