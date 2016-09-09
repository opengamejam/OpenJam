//
//  CRenderTargetStencilOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0)

#ifndef CRENDERTARGETSTENCILOGL1_3_H
#define CRENDERTARGETSTENCILOGL1_3_H

#include "IRenderTarget.h"

namespace jam
{

class CRenderTargetStencilOGL1_3 : public CRenderTargetStencil
{
    JAM_OBJECT
public:
    CRenderTargetStencilOGL1_3();
    virtual ~CRenderTargetStencilOGL1_3();
    
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
     * OGL1_3 specific
     */
    void BindToFrameBuffer();
    void UnbindFromFrameBuffer();
    
private:
    uint32_t m_Id;
};
    
}; // namespace jam

#endif /* CRENDERTARGETSTENCILOGL1_3_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) */
