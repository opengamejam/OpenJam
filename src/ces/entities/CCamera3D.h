//
//  CCamera3D.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CCAMERA3D_H
#define CCAMERA3D_H

#include "ICamera.h"

namespace jam
{
CLASS_PTR(CCamera3D);

class CCamera3D : public ICamera
{
public:
    static CCamera3DPtr Create(float fov, float width, float height, float near = 0.3f, float far = 1000.0f);
    
    CCamera3D(float fov, float width, float height, float near = 0.3f, float far = 1000.0f);
    virtual ~CCamera3D();
    
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
};

}; // namespace jam

#endif /* CCAMERA3D_H */
