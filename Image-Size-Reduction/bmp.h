#pragma once

#include <globals.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "RLECompression.h"

typedef struct  tagBITMAPFILEHEADER {
    u16 file_type;          // Type of file BM is 0x4D42
    u32 file_size;          // Size of file in bytes
    u16 reserved_data1;     // Reserved area, always 0
    u16 reserved_data2;     // Reserved area, always 0
    u32 data_offset;        // Starting position of pixel data
} BITMAPFILEHEADER, * PTRBITMAPFILEHEADER;

typedef struct  tagBITMAPINFOHEADER {
    u32 size;       // Size of header in bytes
    i32 width;      // Width of bitmap in pixels
    i32 height;     // Height of bitmap in pixels

    u16 planes;     // Number of target device, always 1
    u16 bit_count;  // Number of bits per pixels
    /*  Bitcount types:
        1 - monochrome
        4 - 16 colors
        8 - 256 colors
        16 - 16 bit color bitmap (HIGH COLOR)
        24 - 24 bit color bitmap (TRUE COLOR)
        32 - 32 bit color bitmap (TRUE COLOR)
    */

    u32 compression;    // Information about used compression
    /*  Compressions:
        0 - none 
        1 - RLE 8 bit on pixel 
        2 - RLE 4 bit on pixel 
        3 - bitfields 
    */

    u32 size_image;   // Must be rounded towards nearest

    i32 x_pixels_per_meter;     // Horizontal resolution per pixel/meter
    i32 y_pixels_per_meter;     // Vertical resolution per pixel/meter

    u32 colors_used;            // Number of color indexes in bitmap
    u32 colors_important;       // Number of colors used to display bitmap
} BITMAPINFOHEADER, * PTRBITMAPINFOHEADER;


typedef struct tagBMPColorHeader {
    u32 red_mask{ 0x00ff0000 };            // Bit mask for the red channel
    u32 green_mask{ 0x0000ff00 };          // Bit mask for the green channel
    u32 blue_mask{ 0x000000ff };           // Bit mask for the blue channel
    u32 alpha_mask{ 0xff000000 };          // Bit mask for the alpha channel
    u32 color_space_type{ 0x73524742 };     // Default "sRGB" (0x73524742)
    u32 unused[16]{ 0 };                    // Unused data for sRGB color spacE
} BMPColorHeader, * PTRBMPColorHeader;


class BitMap
{
public:
    BITMAPFILEHEADER bmp_file_header;   // Contains bmp file info
    BITMAPINFOHEADER bmp_info_header;   // Contains bmp header info
    BMPColorHeader bmp_color_header;    // Contains bmp color info
    u8* pixel_data;                           //

    std::string file_destination;       // changed filename to dest file

    RLECompression rle_compression;     // Compresses bmp object


    BitMap(const std::string file_path);
    ~BitMap();

    bool read(const std::string file_path);


    void changeDestFileName(const std::string file_path);


    void compressRLE();

private:



};



