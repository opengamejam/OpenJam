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
    JAM_OBJECT_BASE
public:
    ICamera() = default;
    virtual ~ICamera() = default;

    /*
     * Returns Projection View matrix
     */
    virtual glm::mat4x4 ProjectionMatrix() = 0;
    
    /*
     * Return attached framebuffer
     */
    virtual IFrameBufferPtr FrameBuffer() const = 0;
    
    /*
     * Assign new framebuffer. Projection matrix will be recalculated to current framebuffer width and height
     */
    virtual void FrameBuffer(IFrameBufferPtr frameBuffer) = 0;

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
