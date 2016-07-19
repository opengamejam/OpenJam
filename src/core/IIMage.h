//
//  IImage.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#ifndef IIMAGE_H
#define	IIMAGE_H

#include "IResource.h"

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
    TexelProps(TexelFormats _texelFormat, TexelTypes _texelType, uint32_t _bpp)
    : texelFormat(_texelFormat)
    , texelType(_texelType)
    , bpp(_bpp)
    {}
    
    TexelFormats texelFormat;
    TexelTypes texelType;
    uint32_t bpp;
};
    
class IImage : public IResource
{
    JAM_OBJECT_BASE
public:
    IImage(const std::string& filename)
    : IResource(filename)
    {}
    IImage(const std::string& name, const IResource::TResourceData& data)
    : IResource(name, data)
    {}
    virtual ~IImage() = default;
    
    virtual uint32_t Width() const = 0;
    virtual uint32_t Height() const = 0;
    virtual uint32_t Bpp() const = 0;
    virtual uint32_t MipsCount() const = 0;
	virtual bool IsCompressed() const = 0;
    virtual TexelFormats TexelFormat() const = 0;
    virtual TexelTypes   TexelType() const = 0;
};

}; // namespace jam

#endif	/* IIMAGE_H */

