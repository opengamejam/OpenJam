//
//  CRenderTargetStencilOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CRENDERTARGETSTENCILOGLBASE_H
#define CRENDERTARGETSTENCILOGLBASE_H

#include "IRenderTarget.h"

namespace jam {

class CRenderTargetStencilOGLBase : public CRenderTargetStencil {
    JAM_OBJECT
public:
    CRenderTargetStencilOGLBase();
    virtual ~CRenderTargetStencilOGLBase();

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
     * Get internal format
     */
    virtual InternalFormats InternalFormat() const override;

    /*
     * OpenGL specific
     */
    virtual void BindToFrameBuffer();
    virtual void UnbindFromFrameBuffer();
    virtual void InitializeWithDepthId(uint32_t depthId);
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) = 0;

private:
    uint32_t m_Id;
    InternalFormats m_InternalFormat;
};

}; // namespace jam

#endif /* CRENDERTARGETSTENCILOGLBASE_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
