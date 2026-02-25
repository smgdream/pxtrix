# PGM Specification (P5)
Updated: 09 October 2016

**NAME**  
pgm - Netpbm grayscale image format  

**DESCRIPTION**  
This program is part of Netpbm.  

The PGM format is a lowest common denominator grayscale file format. It is designed to be extremely easy to learn and write programs for. (It's so simple that most people will simply reverse engineer it because it's easier than reading this specification).  

A PGM image represents a grayscale graphic image. There are many pseudo-PGM formats in use where everything is as specified herein except for the meaning of individual pixel values. For most purposes, a PGM image can just be thought of an array of arbitrary integers, and all the programs in the world that think they're processing a grayscale image can easily be tricked into processing something else.  

The name "PGM" is an acronym derived from "Portable Gray Map."  

One official variant of PGM is the transparency mask. A transparency mask in Netpbm is represented by a PGM image, except that in place of pixel intensities, there are opaqueness values. See below.  

## THE FORMAT
The format definition is as follows. You can use the libnetpbm C subroutine library to conveniently and accurately read and interpret the format.  

A PGM file consists of a sequence of one or more PGM images. There are no data, delimiters, or padding before, after, or between images.  

Each PGM image consists of the following:  

1. A "magic number" for identifying the file type. A pgm image's magic number is the two characters "P5".  
2. Whitespace (blanks, TABs, CRs, LFs).  
3. A width, formatted as ASCII characters in decimal.  
4. Whitespace.  
5. A height, again in ASCII decimal.  
6. Whitespace.  
7. The maximum gray value (Maxval), again in ASCII decimal. Must be less than 65536, and more than zero.  
8. A single whitespace character (usually a newline).  
9. A raster of Height rows, in order from top to bottom. Each row consists of Width gray values, in order from left to right. Each gray value is a number from 0 through Maxval, with 0 being black and Maxval being white. Each gray value is represented in pure binary by either 1 or 2 bytes. If the Maxval is less than 256, it is 1 byte. Otherwise, it is 2 bytes. The most significant byte is first.  

A row of an image is horizontal. A column is vertical. The pixels in the image are square and contiguous.  

Each gray value is a number proportional to the intensity of the pixel, adjusted by the ITU-R Recommendation BT.709 gamma transfer function. (That transfer function specifies a gamma number of 2.2 and has a linear section for small intensities). A value of zero is therefore black. A value of Maxval represents CIE D65 white and the most intense value in the image and any other image to which the image might be compared.  

BT.709's range of channel values (16-240) is irrelevant to PGM.  

Note that a common variation from the PGM format is to have the gray value be "linear," i.e. as specified above except without the gamma adjustment. pnmgamma takes such a PGM variant as input and produces a true PGM as output.  

Another popular variation from PGM is to substitute the newer sRGB transfer function for the BT.709 one. You can use pnmgamma to convert between this variation and true PGM.  

In the transparency mask variation from PGM, the value represents opaqueness. It is proportional to the fraction of intensity of a pixel that would show in place of an underlying pixel. So what normally means white represents total opaqueness and what normally means black represents total transparency. In between, you would compute the intensity of a composite pixel of an "under" and "over" pixel as under * (1-(alpha/alpha_maxval)) + over * (alpha/alpha_maxval). Note that there is no gamma transfer function in the transparency mask.  

Strings starting with "#" may be comments, the same as with PBM.  

Note that you can use **pamdepth** to convert between the format with 1 byte per gray value and the one with 2 bytes per gray value.  

All characters referred to herein are encoded in ASCII. "newline" refers to the character known in ASCII as Line Feed or LF. A "white space" character is space, CR, LF, TAB, VT, or FF (I.e. what the ANSI standard C isspace() function calls white space).  

## Reference
[PGM Format Specification](https://netpbm.sourceforge.net/doc/pgm.html)