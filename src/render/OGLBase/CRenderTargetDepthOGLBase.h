//
//  CRenderTargetDepthOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CRENDERTARGETDEPTHOGLBASE_H
#define CRENDERTARGETDEPTHOGLBASE_H

#include "IRenderTarget.h"

namespace jam
{
CLASS_PTR(CRenderTargetStencil);
    

class CRenderTargetDepthOGLBase : public CRenderTargetDepth
{
    JAM_OBJECT
public:
    CRenderTargetDepthOGLBase();
    virtual ~CRenderTargetDepthOGLBase();
    
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
     * Returns combined stencil target
     */
    virtual CRenderTargetStencilPtr StencilTarget() override;
    
    /*
     * OpenGL specific
     */
    virtual void BindToFrameBuffer();
    virtual void UnbindFromFrameBuffer();
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) = 0;
    virtual CRenderTargetStencilPtr CreateStencilObject() = 0;
    
private:
    void CreateStencil();
    void DeleteStencil();
    
private:
    uint32_t m_Id;
    InternalFormats m_InternalFormat;
    CRenderTargetStencilPtr m_Stencil;
};
    
}; // namespace jam

#endif /* CRENDERTARGETDEPTHOGLBASE_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
