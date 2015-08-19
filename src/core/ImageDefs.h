//
//  ImageDefs.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef IMAGEDEFS_H
#define IMAGEDEFS_H

namespace jam
{

enum TexelFormats
{
    TF_A8 = 0,   // Alpha
    TF_L8,       // Luminance
    TF_LA88,
    TF_RGB565,   // Red Green Blue
    TF_RGB888,
    TF_RGBA5551,
    TF_RGBA4444,
    TF_RGBA8888,
    TF_RGBA32323232,
    TF_BGRA8888,
    // Compressed formats
    TF_PVRTC2,
    TF_PVRTC2Alpha,
    TF_PVRTC4,
    TF_PVRTC4Alpha,
    TF_ETC,
    // Twiddled
    TF_RGB565_TWID,
    TF_ARGB1555_TWID,
    TF_ARGB4444_TWID
};

enum TexelTypes
{
    TT_UByte = 0,
    TT_Byte,
    TT_UShort,
    TT_Short,
    TT_UInt,
    TT_Int,
    TT_Float
};

struct TexelProps
{
    TexelProps(TexelFormats _texelFormat, TexelTypes _texelType, unsigned int _bpp)
        : texelFormat(_texelFormat)
        , texelType(_texelType)
        , bpp(_bpp)
    {}
    
    TexelFormats texelFormat;
    TexelTypes texelType;
    unsigned int bpp;
};

}; // namespace jam

#endif /* IMAGEDEFS_H */
