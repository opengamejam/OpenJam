//
//  CFrameBufferRenderTargetOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CFRAMEBUFFERTARGETOGLES1_0_H
#define CFRAMEBUFFERTARGETOGLES1_0_H

#include "IRenderTarget.h"

namespace jam
{
    
class CFrameBufferTargetOGLES1_0 : public IRenderTarget
{
public:
    CFrameBufferTargetOGLES1_0(unsigned int width, unsigned int height);
    virtual ~CFrameBufferTargetOGLES1_0();
    
    void Initialize(unsigned int externalFrameBuffer, unsigned int externalColorBuffer = -1,
                    unsigned int externalDepthBuffer = -1, unsigned int externalStencilBuffer = -1);
    virtual void Initialize();
    
    virtual bool CreateColorAttachment(int index);
    virtual bool CreateDepthAttachment();
    virtual bool CreateStencilAttachment();
    virtual bool CreateTextureAttachment();
    
    virtual void Bind() const;
    virtual void Unbind() const;
    virtual void Clear() const;
    
    virtual void ClearColor(const CColor& color);
    virtual const CColor& ClearColor() const;
    
    virtual unsigned int Width() const;
    virtual unsigned int Height() const;
    
    virtual std::vector<unsigned char> RawData();
    
private:
    unsigned int m_FrameBuffer;
    
    std::vector<unsigned int> m_ColorBuffers;
    unsigned int m_DepthBuffer;
    unsigned int m_StencilBuffer;
    int m_NumColorAtachments;
    
    bool m_IsFrameBufferExt;
    bool m_IsColor0BufferExt;
    bool m_IsDepthBufferExt;
    bool m_IsStencilBufferExt;
    
    unsigned int m_Width;
    unsigned int m_Height;
    
    CColor m_ClearColor;
};

}; // namespace jam

#endif /* defined(CFRAMEBUFFERTARGETOGLES1_0_H) */

#endif /* RENDER_OGLES1 */