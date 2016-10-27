//
//  CRenderTargetTextureOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#ifndef CRENDERTARGETTEXTUREOGL1_5_H
#define CRENDERTARGETTEXTUREOGL1_5_H

#include "IRenderTarget.h"

namespace jam
{

class CRenderTargetTextureOGL1_5 : public CRenderTargetTexture
{
    JAM_OBJECT
public:
    CRenderTargetTextureOGL1_5();
    virtual ~CRenderTargetTextureOGL1_5();
    
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
     * OGL1_5 specific
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

#endif /* CRENDERTARGETTEXTUREOGL1_5_H */

#endif /* defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) */
