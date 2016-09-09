//
//  CRenderTargetTextureOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0)

#ifndef CRENDERTARGETTEXTUREOGL1_3_H
#define CRENDERTARGETTEXTUREOGL1_3_H

#include "IRenderTarget.h"

namespace jam
{

class CRenderTargetTextureOGL1_3 : public CRenderTargetTexture
{
    JAM_OBJECT
public:
    CRenderTargetTextureOGL1_3();
    virtual ~CRenderTargetTextureOGL1_3();
    
    /*
     * Initialize render target object
     */
    virtual void Initialize() override;
    
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
     * OGL1_3 specific
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

#endif /* CRENDERTARGETTEXTUREOGL1_3_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) */
