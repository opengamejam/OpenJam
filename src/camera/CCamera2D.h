//
//  CCamera2D.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CCAMERA2D_H
#define CCAMERA2D_H

#include "ICamera.h"

namespace jam
{

class CCamera2D : public ICamera
{
public:
    CCamera2D(float width, float height, float near = -10000.0f, float far = 10000.0f);
    virtual ~CCamera2D();
    
    virtual CMatrix4x4f ProjectionMatrix();
    virtual IRenderTargetPtr RenderTarget() const;
    virtual void RenderTarget(IRenderTargetPtr renderTarget);
    
    virtual void FlipY();
    virtual void FlipX();
    
private:
    float m_Width;
    float m_Height;
    float m_Near;
    float m_Far;
    CMatrix4x4f m_ProjectionMatrix;
    IRenderTargetPtr m_RenderTarget;
    
    bool m_IsFlippedX;
    bool m_IsFlippedY;
};

}; // namespace jam

#endif /* CCAMERA2D_H */
