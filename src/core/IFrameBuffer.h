 
//
//  IFrameBuffer.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#ifndef IFrameBuffer_H
#define IFrameBuffer_H

#include "Global.h"
#include "CColor.hpp"

namespace jam {
CLASS_PTR(IFrameBuffer)
CLASS_PTR(IRenderTarget)

class IFrameBuffer {
    JAM_OBJECT_BASE
public:
    typedef std::vector<uint8_t> TRawData;

public:
    /*
     * Initialize framebuffer object
     */
    virtual void Initialize() = 0;

    /*
     * Deinitialize framebuffer object
     */
    virtual void Shutdown() = 0;

    /*
     * Check if framebuffer is initialized
     */
    virtual bool IsInitialized() = 0;

    /*
     * Resize frame buffer
     */
    virtual void Resize(uint64_t width, uint64_t height) = 0;

    /* 
     * Get maximum color attachements
     */
    virtual uint64_t MaxColorAttachements() const = 0;

    /*
     * Attach render buffer or render texture as color buffer. 
     * Setup index to specify multiple render target
     */
    virtual void AttachColor(IRenderTargetPtr colorTarget, uint64_t index) = 0;

    /*
     * Attach render buffer or render texture as depth buffer
     */
    virtual void AttachDepth(IRenderTargetPtr colorTarget) = 0;

    /*
     * Attach render buffer or render texture as stencil buffer
     */
    virtual void AttachStencil(IRenderTargetPtr colorTarget) = 0;

    /*
     * Detach color buffer
     */
    virtual void DetachColor(uint64_t index) = 0;

    /*
     * Detach depth buffer
     */
    virtual void DetachDepth() = 0;

    /*
     * Detach stencil buffer
     */
    virtual void DetachStencil() = 0;

    /*
     * Returns attched color buffer for index
     */
    virtual IRenderTargetPtr ColorAttachement(uint64_t index) const = 0;

    /*
     * Returns attched depth buffer
     */
    virtual IRenderTargetPtr DepthAttachement() const = 0;

    /*
     * Returns attched stencil buffer
     */
    virtual IRenderTargetPtr StencilAttachement() const = 0;

    /*
     * Check if framebuffer is valid. For ex. OpenGL requires at least one
     * attached color buffers to be a valid framebuffer
     */
    virtual bool IsValid() const = 0;

    /*
     * Bind current framebuffer
     */
    virtual void Bind() const = 0;

    /*
     * Unbind framebuffer
     */
    virtual void Unbind() const = 0;

    /*
     * Clear all attached renderbuffers with color 'ClearColor'
     */
    virtual void Clear() const = 0;

    /*
     * Returns framebuffer width
     */
    virtual uint64_t Width() const = 0;

    /*
     * Returns framebuffer height
     */
    virtual uint64_t Height() const = 0;

    /*
     * Setup clear color which will fill color buffer after clear
     */
    virtual void ClearColor(const CColor4f& color) = 0;

    /*
     * Returns current clear color
     */
    virtual const CColor4f& ClearColor() const = 0;

    /*
     * Extract framebuffer raw image data (TODO: format)
     */
    virtual TRawData RawData() = 0;
};

}; // namespace jam

#endif /* IFrameBuffer_H */

