//
//  ICamera.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#ifndef ICAMERA_H
#define ICAMERA_H

#include "IEntity.h"

namespace jam {
CLASS_PTR(IFrameBuffer)

class ICamera : public CEntityBase {
public:
    static const uint64_t kDefaultFrameBuffer = 0;
    
public:
    ICamera() = default;
    virtual ~ICamera() = default;

    /*
     * Returns Projection View matrix. Attribute 'frameBufferIndex' choose projection matrix for according frame buffer
     * if there are several atached frame buffers
     */
    virtual glm::mat4x4 ProjectionMatrix(uint64_t frameBufferIndex = kDefaultFrameBuffer) = 0;
    
    /*
     * Return attached framebuffer. Attribute 'frameBufferIndex' choose required frame buffer
     * if there are several atached frame buffers
     */
    virtual IFrameBufferPtr FrameBuffer(uint64_t frameBufferIndex = kDefaultFrameBuffer) const = 0;
    
    /*
     * Assign new framebuffer. Projection matrix will be recalculated to current framebuffer width and height.
     * Attribute 'frameBufferIndex' choose required frame buffer if there are several atached frame buffers
     */
    virtual void FrameBuffer(IFrameBufferPtr frameBuffer, uint64_t frameBufferIndex = kDefaultFrameBuffer) = 0;
    
    /*
     * Returns count of attached frame buffers
     */
    virtual uint64_t FrameBufferCount() const = 0;

    /*
     * Flip camera vertically
     */
    virtual void FlipY() = 0;
    
    /*
     * Flip camera horizontally
     */
    virtual void FlipX() = 0;
};

}; // namespace jam

#endif /* defined(ICAMERA_H) */
