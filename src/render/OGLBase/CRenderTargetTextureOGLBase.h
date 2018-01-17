//
//  CRenderTargetTextureOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CRENDERTARGETTEXTUREOGLBASE_H
#define CRENDERTARGETTEXTUREOGLBASE_H

#include "IRenderTarget.h"

namespace jam {
CLASS_PTR(ITexture)

class CRenderTargetTextureOGLBase : public CRenderTargetTexture {
    JAM_OBJECT
public:
    CRenderTargetTextureOGLBase();
    virtual ~CRenderTargetTextureOGLBase();

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
     * Get render texture
     */
    virtual ITexturePtr Texture() const override;

    /*
     * OpenGL specific
     */
    virtual void BindAsColorToFrameBuffer(uint64_t colorAttachementIdx);
    virtual void BindAsDepthToFrameBuffer();
    virtual void BindAsStencilToFrameBuffer();

    virtual void UnbindAsColorFromFrameBuffer(uint64_t colorAttachementIdx);
    virtual void UnbindAsDepthFromFrameBuffer();
    virtual void UnbindAsStencilFromFrameBuffer();
    
    virtual ITexturePtr CreateTextureObject() const = 0;

private:
    ITexturePtr m_Texture;
};

}; // namespace jam

#endif /* CRENDERTARGETTEXTUREOGLBASE_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
