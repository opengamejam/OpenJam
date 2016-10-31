//
//  CRenderTargetColorOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CRENDERTARGETCOLOROGLBASE_H
#define CRENDERTARGETCOLOROGLBASE_H

#include "IRenderTarget.h"

namespace jam
{

class CRenderTargetColorOGLBase : public CRenderTargetColor
{
    JAM_OBJECT
public:
    CRenderTargetColorOGLBase();
    virtual ~CRenderTargetColorOGLBase();
    
    /*
     * Initialize render target object
     */
    virtual void Initialize(InternalFormats internalFormat) override;
    
    /*
     * Deinitialize render target object
     */
    virtual void Shutdown() override;
    
    /*
     * Check if render target is initialized
     */
    virtual bool IsInitialized() override;
    
    /*
     * Allocate render buffer with 'width' and 'height'
     */
    virtual void Allocate(uint64_t width, uint64_t height) override;
    
    /*
     * Bind current render target
     */
    virtual void Bind() const override;
    
    /*
     * Unbind current render target
     */
    virtual void Unbind() const override;
    
    /*
     * OpenGL specific
     */
    virtual void BindToFrameBuffer(uint64_t colorAttachementIdx);
    virtual void UnbindFromFrameBuffer(uint64_t colorAttachementIdx);
    virtual GLenum ConvertInternalFormat(InternalFormats internalFormat) = 0;
    
private:
    uint32_t m_Id;
    InternalFormats m_InternalFormat;
};
    
}; // namespace jam

#endif /* CRENDERTARGETCOLOROGLBASE_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
