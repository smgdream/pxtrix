# Introduction of tests

tests组件用于对pxtrix各项功能进行测试。tests中包含了多个测试用例，各测试用例的主要测试对象和附注见下表： 

| 测试用例名| 主要测试对象 | 附注 |
| --------- | --------- | - |
| bmpimg	| Bmpimg	| / |
| pixel		| Pixel		| / |
| image0	| Image		| / |
| qoimg		| Qoimg		| / |
| pgm		| Pgm		| / |
| fbdev		| Fbdev		| 需要在支持Freambuffer的环境中进行测试 |
| chart		| Chart		| / |
| render	| Render	| / |
| luts		| Luts		| / |
| util		| Utility	| 需要使用支持C23的编译器才可进行完整测试 |
| image1	| Image		| 用于测试Image的特殊情况处理 |
<!-- |  |  |  | -->

使用make test命令即可编译并执行各测试用例，一个测试用例返回非零值表示测试失败（非零的返回值对应出错返回处的行号）。如需进行完整的测试需要在Framebuffer功能可用的Linux本地tty终端中使用支持C23的编译器将pxtrix编译为静态库并通过make test执行测试。如果环境不符合完整测试的环境要求，则仅会执行受限的测试。  

注：在Linux可通过`ctrl+atl+功能键`在多个tty中切换。  