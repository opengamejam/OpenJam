//
//  CRenderTargetDepthOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CRENDERTARGETDEPTHOGL2_0_H
#define CRENDERTARGETDEPTHOGL2_0_H

#include "IRenderTarget.h"

namespace jam
{

class CRenderTargetDepthOGL2_0 : public CRenderTargetDepth
{
    JAM_OBJECT
public:
    CRenderTargetDepthOGL2_0();
    virtual ~CRenderTargetDepthOGL2_0();
    
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
    
    // TODO: depth + stencil
    
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
     * OGL2_0 specific
     */
    void BindToFrameBuffer();
    void UnbindFromFrameBuffer();
    
private:
    uint32_t m_Id;
};
    
}; // namespace jam

#endif /* CRENDERTARGETDEPTHOGL2_0_H */

#endif // defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)
