# Pxtrix

[Pxtrix](https://github.com/smgdream/pxtrix) is a image procressing and drawing kit in C. Pxtrix mean "Pixels Matrix". It can be used to process or draw a picture.  
Author: [smgdream](https://smgdream.com)  
Version: 0.2.4 dev  
Tab: Four space per tab  

## License
[MIT License](LICENSE)  
Copyright (c) 2024-2026 Smgdream  

## Compile

```
Enviroment requirement

C version: C99
Compiler: >= gcc-5 or >= tcc-0.28rc ( >= clang-3 may can)
Used: C Complier, printf, find, xargs, uname, cloc, tar, xz, sed, and etc.
NOTE: For Win, tar, find and sed of busybox version have something wrong. 
      I using git version to compile it on Windows.

Compile

$ make [OPTIONS]
Compile the main example

$ make lib [OPTIONS]
make pxtrix as static library

$ make so_lib [OPTIONS]
make pxtrix as shared library (experiment)
Note: Not support for tcc for windows.

Library files are output to the build directory

NOTE: It can not use tcc to compile the library and link it by GNU linker. But
      you can use gcc to compile it and use tcc linker to link. Remake the
      library should make distclean at first.

General options:
-j [N]        allow N jobs at once
V=(0 | 1)     show the datil or not (default: 0)
S=(0 | 1)     static link or not. Just for default target. (If the CC is tcc it
              will be set to 0 at the end, no metter what you set. if the CC is
              not tcc it's 1 as default on Linux, 0 as default on other OS)
DEBUG=(0 | 1) debug or not (default: 0)
CC=COMPILER   the compiler you want (default: CC=gcc)

e.g. $ make -j 16 CC=tcc V=1 S=0

$ make test
Testing pxtrix. (Should download test images archive and extract first. The URL
of test images archive can available at the end of README.)
Note: The compiler you used should same as make lib.

$ make clean
Delete all of the intermediate files and output files. (include: *.a, *.o)

$ make distclean
Delete all of  the intermediate files, executable files, image files and build
directory in the pxtrix's project directory. (include: *.a, *.o, *.exe,*.png,
*.bmp, *.qoi, *.pgm, ect.)

$ make count
Count lines of code

$ make dist
package the pxtrix (in NT tar of git version can)
```

### Getting Test Image
```sh
wget https://github.com/smgdream/bed/releases/download/0/pictures-0.1.5.tar.xz
tar -xf pictures-0.1.5.tar.xz -C /path/to/pxtrix/tests
```

### Contributors
- [Wenqi](https://github.com/WenqiOfficial)
- [Andyccr](https://github.com/Andyccr)
