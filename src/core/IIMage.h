//
//  IGameObject.h
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
    virtual unsigned int Width() const = 0;
    virtual unsigned int Height() const = 0;
    virtual unsigned int Bpp() const = 0;
    virtual unsigned int MipsCount() const = 0;
	virtual unsigned int IsCompressed() const = 0;
    virtual TexelFormats TexelFormat() const = 0;
    virtual TexelTypes   TexelType() const = 0;
    
    IImage(const std::string& filename, const std::string& loader)
        : IResource(filename, loader)
    {}
    
    IImage(const std::string& name, const IResource::TResourceData& data)
        : IResource(name, data)
    {}
    
    virtual ~IImage() = default;
};

}; // namespace jam

#endif	/* IIMAGE_H */

