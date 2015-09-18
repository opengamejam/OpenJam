//
//  CFrameBufferOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#ifndef CFRAMEBUFFEROGLES1_0_H
#define CFRAMEBUFFEROGLES1_0_H

#include "IRenderTarget.h"

namespace jam
{
    
class CFrameBufferOGLES1_0 : public IRenderTarget
{
public:
    CFrameBufferOGLES1_0(uint32_t width, uint32_t height);
    virtual ~CFrameBufferOGLES1_0();
    
    void Initialize(uint32_t externalFrameBuffer, uint32_t externalColorBuffer = -1,
                    uint32_t externalDepthBuffer = -1, uint32_t externalStencilBuffer = -1);
    virtual void Initialize() override;
    
    virtual bool CreateColorAttachment(int index) override;
    virtual bool CreateDepthAttachment() override;
    virtual bool CreateStencilAttachment() override;
    virtual bool CreateTextureAttachment() override;
    
    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void Clear() const override;
    
    virtual void ClearColor(const CColor& color) override;
    virtual const CColor& ClearColor() const override;
    
    virtual uint32_t Width() const override;
    virtual uint32_t Height() const override;
    
    virtual TRawData RawData() override;
    
private:
    uint32_t m_FrameBuffer;
    
    std::vector<uint32_t> m_ColorBuffers;
    uint32_t m_DepthBuffer;
    uint32_t m_StencilBuffer;
    int m_NumColorAtachments;
    
    bool m_IsFrameBufferExt;
    bool m_IsColor0BufferExt;
    bool m_IsDepthBufferExt;
    bool m_IsStencilBufferExt;
    
    uint32_t m_Width;
    uint32_t m_Height;
    
    CColor m_ClearColor;
};

}; // namespace jam

#endif /* defined(CFRAMEBUFFEROGLES1_0_H) */

#endif /* RENDER_OGLES1_1 */