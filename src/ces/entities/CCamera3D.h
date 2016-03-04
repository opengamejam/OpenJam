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
#include "CMath.h"

namespace jam
{
CLASS_PTR(CCamera3D);

class CCamera3D : public ICamera
{
public:
    static CCamera3DPtr Create(float _fov, float _width, float _height, float _near = 0.3f, float _far = 1000.0f);
    
    CCamera3D(float _fov, float _width, float _height, float _near = 0.3f, float _far = 1000.0f);
    virtual ~CCamera3D();
    
    virtual uint32_t Id() const override;
    
    virtual glm::mat4x4 ProjectionMatrix() override;
    virtual IRenderTargetPtr RenderTarget() const override;
    virtual void RenderTarget(IRenderTargetPtr renderTarget) override;
    
    virtual void FlipY() override;
    virtual void FlipX() override;
    
private:
    float m_Width;
    float m_Height;
    float m_Near;
    float m_Far;
    glm::mat4x4 m_ProjectionMatrix;
    IRenderTargetPtr m_RenderTarget;
    
    uint32_t m_Id;
};

}; // namespace jam

#endif /* CCAMERA3D_H */
