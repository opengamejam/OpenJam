//
//  CTextureRenderTarget.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if 0

#ifndef CTEXTURERENDERTARGET_H
#define CTEXTURERENDERTARGET_H

#include "CFrameBufferRenderTarget.h"

namespace jam
{
CLASS_PTR(ITexture)
    
class CTextureRenderTarget : public CFrameBufferRenderTarget
{
public:
    CTextureRenderTarget(unsigned int width, unsigned int height);
    virtual ~CTextureRenderTarget();
    
    ITexturePtr Texture() const;
    void SaveImage(const std::string& filepath);
    
private:
    ITexturePtr m_Texture;
};

}; // namespace jam

#endif /* defined(CTEXTURERENDERTARGET_H) */

#endif