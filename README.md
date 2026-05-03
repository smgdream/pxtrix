# Pxtrix

[Pxtrix](https://github.com/smgdream/pxtrix) is an image processing and drawing toolkit written in C. The name “Pxtrix” stands for **Pixels Matrix**.
**Author:** [smgdream](https://smgdream.com)  
**Version:** 0.2.4 dev  
**Tab width:** four spaces

## License

[MIT License](LICENSE)  
Copyright (c) 2024–2026 Smgdream

## Compile

```
Environment Requirements

C standard: C99
Compiler: >= gcc-5 or >= tcc-0.28rc (>= clang-3 may also work)
Required tools: C compiler, printf, find, xargs, uname, cloc, tar, xz, sed, etc.

NOTE:
On Windows, the busybox versions of tar, find, and sed may not function correctly.
The Git-provided versions are recommended for building on Windows.


Build Commands

$ make [OPTIONS]
Build the main example program.

$ make lib [OPTIONS]
Build pxtrix as a static library.

$ make so_lib [OPTIONS]
Build pxtrix as a shared library (experimental).
Note: Not supported for tcc on Windows.

Library files are output to the build directory.

NOTE:
You cannot use tcc to compile the library and then link it using the GNU linker.
However, you can compile with gcc and link with tcc.
When rebuilding the library, run `make distclean` first.


General Options

-j [N]        Allow N jobs to run in parallel.
V=(0 | 1)     Show build details (default: 0).
S=(0 | 1)     Enable static linking (default depends on compiler and OS):
              If CC is tcc: forced to 0.
              If CC is not tcc: default is 1 on Linux, 0 on other OS.
DEBUG=(0 | 1) Enable debug mode (default: 0).
CC=COMPILER   Specify the compiler (default: gcc).

e.g. $ make -j 16 CC=tcc V=1 S=0


$ make test
Run pxtrix tests.
(Test image archive must be downloaded and extracted first.
The download URL is provided at the end of this README.)
Note: Use the same compiler as was used for `make lib`.

$ make clean
Delete intermediate and output files (including: *.a, *.o).

$ make distclean
Delete intermediate files, executables, image files, and the build directory
(including: *.a, *.o, *.exe, *.png, *.bmp, *.qoi, *.pgm, etc.).

$ make count
Count lines of code.

$ make dist
Package the pxtrix project (works with the Git-provided tar on Windows).
```

## Getting Test Images

```sh
wget https://github.com/smgdream/bed/releases/download/0/pictures-0.1.5.tar.xz
tar -xf pictures-0.1.5.tar.xz -C /path/to/pxtrix/tests
```

## Contributors

- [Wenqi](https://github.com/WenqiOfficial)
- [Andyccr](https://github.com/Andyccr)
