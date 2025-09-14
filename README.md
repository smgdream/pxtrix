# Pxtrix
[Pxtrix](https://github.com/smgdream/pxtrix) is a image procressing and drawing kit in C. Pxtrix mean "Pixels Matrix". It can be used to process or draw a picture.  
Author: [smgdream](https://smgdream.com)  
Version: 0.2.2 dev 17-1  
Tab: 4space per tab  

## License
[MIT License](LICENSE)  
Copyright (c) 2024 Smgdream  

## Compile

```sh
# C version: C99
# Compiler: >= gcc-5 or >= tcc-0.28rc ( >= clang-3 may can)
# Used: C Complier, printf, find, xargs, uname, cloc, tar, xz, sed, and etc.
# NOTE: For Win, tar and sed of busybox version have something wrong. I
#       using git version to compile it in Win.


# Compile
make [OPTIONS]
# make pxtrix as static library
make lib [OPTIONS]
# make pxtrix as shared library (experiment)
make so_lib [OPTIONS]
# Lib files are output to the build directory
# NOTE: It can not use tcc to compile the library and link it by GNU linker.
# 	But you can use gcc to compile it and use tcc linker to link.Remake the
#	library should make cleanall at first.
#
# General options:
# -j [N]		allow N jobs at once
# V=(0 | 1)		show the datil or not (default: 0)
# S=(0 | 1)		static link or not (if the CC is tcc it will be set to 0 in
# 				the end, no metter what you set. if the CC is not tcc it's 1
#				as default)
# CC=COMPILER	the compiler you want (default: CC=gcc)
#
# e.g. make -j 16 CC=tcc V=1 S=0

# Delete all of the intermediate file. (include: *.a, *.o)
make clean

# Delete all of  the intermediate file, executable file, image file and
# build directory in the pxtrix's project directory. (include: *.a, *.o, 
# *.exe,*.png, *.bmp, *.qoi, *.pgm)
make distclean

# Count lines of code
make count

# package the pxtrix (in NT tar of git version can)
make dist
or
fakeroot make dist
```

### Contributors
- [Wenqi](https://github.com/WenqiOfficial)
