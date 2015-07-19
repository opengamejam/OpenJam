//
//  ICamera.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#ifndef ICAMERA_H
#define ICAMERA_H

#include "IGameObject.h"

namespace jam
{
CLASS_PTR(IRenderTarget);
    
class ICamera : public IGameObject
{
public:
    ICamera()
    : IGameObject("camera")
    {}
    
    virtual ~ICamera() = default;
    
    virtual CMatrix4x4f ProjectionMatrix() = 0;
    virtual IRenderTargetPtr RenderTarget() const = 0;
    virtual void RenderTarget(IRenderTargetPtr renderTarget) = 0;
    
    virtual void FlipY() = 0;
    virtual void FlipX() = 0;
};

}; // namespace jam

#endif /* defined(ICAMERA_H) */
