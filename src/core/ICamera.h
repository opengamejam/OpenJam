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

namespace jam
{
CLASS_PTR(IFrameBuffer)
    
class ICamera : public CEntityBase
{
    JAM_OBJECT_BASE
public:
    ICamera() = default;
    virtual ~ICamera() = default;
    
    static uint32_t NextCameraId()
    {
        static uint32_t nextId;
        return nextId++;
    }
    
    virtual uint32_t Id() const = 0;
    
    virtual glm::mat4x4 ProjectionMatrix() = 0;
    virtual IFrameBufferPtr RenderTarget() const = 0;
    virtual void RenderTarget(IFrameBufferPtr renderTarget) = 0;
    
    virtual void FlipY() = 0;
    virtual void FlipX() = 0;
};

}; // namespace jam

#endif /* defined(ICAMERA_H) */
