//
//  IRenderTarget.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 8/31/16.
//
//

#ifndef IRENDERTARGET_H
#define IRENDERTARGET_H

#include "Global.h"

namespace jam
{
CLASS_PTR(ITexture)
CLASS_PTR(IRenderTargetColor)
CLASS_PTR(CRenderTargetColor)
CLASS_PTR(CRenderTargetDepth)
CLASS_PTR(CRenderTargetStencil)
CLASS_PTR(CRenderTargetTexture)
    
class IRenderTarget : public std::enable_shared_from_this<IRenderTarget>
{
    JAM_OBJECT_BASE
public:
    enum InternalFormats
    {
        ColorR8         = 0x10,
        ColorR16        = 0x11,
        ColorR32        = 0x12,
        ColorRGB888     = 0x13,
        ColorRGB565     = 0x14,
        ColorRGBA8888   = 0x15,
        ColorRGBA4444   = 0x16,
        ColorRGB10_A2   = 0x17,
        Depth16         = 0x80,
        Depth24         = 0x81,
        Depth32         = 0x82,
        Stencil8        = 0x100,
        Depth24_Stencil8 = Depth24 | Stencil8,
    };
    
    /*
     * Initialize render target object
     */
    virtual void Initialize(InternalFormats internalFormat) = 0;
    
    /*
     * Deinitialize render target object
     */
    virtual void Shutdown() = 0;
    
    /*
     * Check if render target is initialized
     */
    virtual bool IsInitialized() = 0;
    
    /*
     * If current render buffer presents color buffer then return self
     */
    virtual CRenderTargetColorPtr ColorTarget() { return nullptr; }
    
    /*
     * If current render buffer presents depth buffer then return self
     */
    virtual CRenderTargetDepthPtr DepthTarget() { return nullptr; }
    
    /*
     * If current render buffer presents stencil buffer then return self
     */
    virtual CRenderTargetStencilPtr StencilTarget() { return nullptr; }
    
    /*
     * If current render buffer presents render texture then return self
     */
    virtual CRenderTargetTexturePtr TextureTarget() { return nullptr; }
    
    /*
     * Bind current render target
     */
    virtual void Bind() const = 0;
    
    /*
     * Unbind current render target
     */
    virtual void Unbind() const = 0;
};

    
class CRenderTargetColor : public IRenderTarget
{
    JAM_OBJECT
public:
    CRenderTargetColor() = default;
    virtual ~CRenderTargetColor() = default;
    
    /*
     * Returns self as color target
     */
    virtual CRenderTargetColorPtr ColorTarget() override
    {
        return std::static_pointer_cast<CRenderTargetColor>(shared_from_this());
    }
    
    /*
     * Allocate render buffer with 'width' and 'height'
     */
    virtual void Allocate(uint64_t width, uint64_t height) = 0;
};
 
    
class CRenderTargetDepth : public IRenderTarget
{
    JAM_OBJECT
public:
    CRenderTargetDepth() = default;
    virtual ~CRenderTargetDepth() = default;
    
    /*
     * Returns self as depth target
     */
    virtual CRenderTargetDepthPtr DepthTarget() override
    {
        return std::static_pointer_cast<CRenderTargetDepth>(shared_from_this());
    }
    
    /*
     * Allocate render buffer with 'width' and 'height'
     */
    virtual void Allocate(uint64_t width, uint64_t height) = 0;
};

    
class CRenderTargetStencil : public IRenderTarget
{
    JAM_OBJECT
public:
    CRenderTargetStencil() = default;
    virtual ~CRenderTargetStencil() = default;
    
    /*
     * Returns self as stencil target
     */
    virtual CRenderTargetStencilPtr StencilTarget() override
    {
        return std::static_pointer_cast<CRenderTargetStencil>(shared_from_this());
    }
    
    /*
     * Allocate render buffer with 'width' and 'height'
     */
    virtual void Allocate(uint64_t width, uint64_t height) = 0;
};
    

class CRenderTargetTexture : public IRenderTarget
{
    JAM_OBJECT
public:
    CRenderTargetTexture() = default;
    virtual ~CRenderTargetTexture() = default;
    
    /*
     * Returns self as render texture target
     */
    virtual CRenderTargetTexturePtr TextureTarget() override
    {
        return std::static_pointer_cast<CRenderTargetTexture>(shared_from_this());
    }
    
    /*
     * Get render texture
     */
    virtual ITexturePtr Texture() const = 0;
    
    /*
     * Set render texture
     */
    virtual void Texture(ITexturePtr texture) = 0;
};
    
}; // namespace jam

#endif /* IRENDERTARGET_H */
