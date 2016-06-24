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
#include "ImageDefs.h"

namespace jam
{

class IImage : public IResource
{
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

