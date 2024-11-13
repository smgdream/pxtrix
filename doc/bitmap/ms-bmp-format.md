# The Microsoft Bitmap Image Format

## Bitmap Storage
The bitmap file format is shown in the following table.  
| Bitmap Image      |
| :---------------: |
| BITMAPFILEHEADER  |
| BITMAPINFOHEADER  |
| RGBQUAD array     |
| Color-index array |

<br>

## BITMAPFILEHEADER structure
```c
typedef struct tagBITMAPFILEHEADER {
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;
```
### Members

`bfType`  
The file type; must be 0x4d42 (the ASCII string "BM").  

`bfSize`  
The size, in bytes, of the bitmap file.  

`bfReserved1`  
Reserved; must be zero.  

`bfReserved2`  
Reserved; must be zero.  

`bfOffBits`  
The offset, in bytes, from the beginning of the BITMAPFILEHEADER structure to the bitmap bits.  
<br>

## BITMAPINFOHEADER structure
```c
typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;
```

### Members

`biSize`  
Specifies the number of bytes required by the structure. This value does not include the size of the color table or the size of the color masks, if they are appended to the end of structure. See Remarks.  

`biWidth`
Specifies the width of the bitmap, in pixels. For information about calculating the stride of the bitmap, see Remarks.

`biHeight`  
Specifies the height of the bitmap, in pixels.  

For uncompressed RGB bitmaps, if biHeight is positive, the bitmap is a bottom-up DIB with the origin at the lower left corner. If biHeight is negative, the bitmap is a top-down DIB with the origin at the upper left corner.  
For YUV bitmaps, the bitmap is always top-down, regardless of the sign of biHeight. Decoders should offer YUV formats with positive biHeight, but for backward compatibility they should accept YUV formats with either positive or negative biHeight.  
For compressed formats, biHeight must be positive, regardless of image orientation.  

`biPlanes`  
Specifies the number of planes for the target device. This value must be set to 1.  

`biBitCount`  
Specifies the number of bits per pixel (bpp). For uncompressed formats, this value is the average number of bits per pixel. For compressed formats, this value is the implied bit depth of the uncompressed image, after the image has been decoded.  

`biCompression`  
For compressed video and YUV formats, this member is a FOURCC code, specified as a DWORD in little-endian order. For example, YUYV video has the FOURCC 'VYUY' or 0x56595559. For more information, see FOURCC Codes.  

For uncompressed RGB formats, the following values are possible:  

| Value        | Meaning                                     |
| ------------ | ------------------------------------------- |
| BI_RGB       | Uncompressed RGB.                           |
| BI_BITFIELDS | Uncompressed RGB with color masks. Valid for 16-bpp and 32-bpp bitmaps. |
 
See Remarks for more information. Note that BI_JPG and BI_PNG are not valid video formats.  

For 16-bpp bitmaps, if biCompression equals BI_RGB, the format is always RGB 555. If biCompression equals BI_BITFIELDS, the format is either RGB 555 or RGB 565. Use the subtype GUID in the AM_MEDIA_TYPE structure to determine the specific RGB type.

`biSizeImage`  
Specifies the size, in bytes, of the image. This can be set to 0 for uncompressed RGB bitmaps.  

`biXPelsPerMeter`  
Specifies the horizontal resolution, in pixels per meter, of the target device for the bitmap.  

`biYPelsPerMeter`  
Specifies the vertical resolution, in pixels per meter, of the target device for the bitmap.  

`biClrUsed`  
Specifies the number of color indices in the color table that are actually used by the bitmap. See Remarks for more information.  

`biClrImportant`  
Specifies the number of color indices that are considered important for displaying the bitmap. If this value is zero, all colors are important.  
<br>

## RGBQUAD structure

Ignore here.  
If you want to know more about this, visit:   
https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-rgbquad\

## Color-index array

```c
/* Microsoft didn't define a specific type of color-index */
struct Colors {
	BYTE blue;
	BYTE green;
	BYTE red;
};
```
The scan lines are DWORD aligned, except for RLE-compressed bitmaps. They must be padded for scan line widths, in bytes, that are not evenly divisible by four, except for RLE compressed bitmaps. For example, a 10- by 10-pixel 24-bpp bitmap will have two padding bytes at the end of each scan line. We can use 0x00 as padding-byte.

## Addition
```c
typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t  LONG;
```

## Reference
https://learn.microsoft.com/en-us/windows/win32/gdi/bitmaps