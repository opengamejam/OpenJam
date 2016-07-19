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
#include "CColor.hpp"

namespace jam
{

class IRenderTarget
{
    JAM_OBJECT_BASE
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
    
    virtual uint32_t Width() const = 0;
    virtual uint32_t Height() const = 0;
    
    virtual void ClearColor(const CColor4f& color) = 0;
    virtual const CColor4f& ClearColor() const = 0;
    
    virtual TRawData RawData() = 0;
};

}; // namespace jam

#endif /* IRENDERTARGET_H */
