//
//  CRenderTargetStencilOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#ifndef CRENDERTARGETSTENCILOGL1_5_H
#define CRENDERTARGETSTENCILOGL1_5_H

#include "IRenderTarget.h"

namespace jam
{

class CRenderTargetStencilOGL1_5 : public CRenderTargetStencil
{
    JAM_OBJECT
public:
    CRenderTargetStencilOGL1_5();
    virtual ~CRenderTargetStencilOGL1_5();
    
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
     * OGL1_5 specific
     */
    void BindToFrameBuffer();
    void UnbindFromFrameBuffer();
    void InitializeWithDepthId(uint32_t depthId);
    
private:
    uint32_t m_Id;
    GLenum m_InternalFormat;
};
    
}; // namespace jam

#endif /* CRENDERTARGETSTENCILOGL1_5_H */

#endif /* defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) */
