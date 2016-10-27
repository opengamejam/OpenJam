//
//  CRenderTargetTextureOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CRENDERTARGETTEXTUREOGL2_0_H
#define CRENDERTARGETTEXTUREOGL2_0_H

#include "IRenderTarget.h"

namespace jam
{

class CRenderTargetTextureOGL2_0 : public CRenderTargetTexture
{
    JAM_OBJECT
public:
    CRenderTargetTextureOGL2_0();
    virtual ~CRenderTargetTextureOGL2_0();
    
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
     * Bind current render target
     */
    virtual void Bind() const override;
    
    /*
     * Unbind current render target
     */
    virtual void Unbind() const override;
    
    /*
     * Get render texture
     */
    virtual ITexturePtr Texture() const override;
    
    /*
     * Set render texture
     */
    virtual void Texture(ITexturePtr texture) override;
    
    /*
     * OGL2_0 specific
     */
    void BindAsColorToFrameBuffer(uint64_t colorAttachementIdx);
    void BindAsDepthToFrameBuffer();
    void BindAsStencilToFrameBuffer();
    
    void UnbindAsColorFromFrameBuffer(uint64_t colorAttachementIdx);
    void UnbindAsDepthFromFrameBuffer();
    void UnbindAsStencilFromFrameBuffer();
    
private:
    uint32_t m_Id;
};
    
}; // namespace jam

#endif /* CRENDERTARGETTEXTUREOGL2_0_H */

#endif // defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)
