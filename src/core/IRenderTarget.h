//
//  IRenderTarget.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#ifndef IRENDERTARGET_H
#define IRENDERTARGET_H

#include "Global.h"
#include "CColor.h"

namespace jam
{

class IRenderTarget
{
public:
    typedef std::vector<uint8_t> TRawData;
    
public:
    virtual void Initialize() = 0;
    
    virtual bool CreateColorAttachment(int index) = 0;
    virtual bool CreateDepthAttachment() = 0;
    virtual bool CreateStencilAttachment() = 0;
    virtual bool CreateTextureAttachment() = 0;
    
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void Clear() const = 0;
    
    virtual unsigned int Width() const = 0;
    virtual unsigned int Height() const = 0;
    
    virtual void ClearColor(const CColor& color) = 0;
    virtual const CColor& ClearColor() const = 0;
    
    virtual TRawData RawData() = 0;
};

}; // namespace jam

#endif /* IRENDERTARGET_H */
