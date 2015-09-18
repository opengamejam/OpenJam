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
CLASS_PTR(CCamera2D);

class CCamera2D : public ICamera
{
public:
    static CCamera2DPtr Create(float width, float height, float near = -10000.0f, float far = 10000.0f);
    
    CCamera2D(float width, float height, float near = -10000.0f, float far = 10000.0f);
    virtual ~CCamera2D();
    
    virtual uint32_t Id() const override;
    
    virtual CMatrix4x4f ProjectionMatrix() override;
    virtual IRenderTargetPtr RenderTarget() const override;
    virtual void RenderTarget(IRenderTargetPtr renderTarget) override;
    
    virtual void FlipY() override;
    virtual void FlipX() override;
    
private:
    float m_Width;
    float m_Height;
    float m_Near;
    float m_Far;
    CMatrix4x4f m_ProjectionMatrix;
    IRenderTargetPtr m_RenderTarget;
    
    bool m_IsFlippedX;
    bool m_IsFlippedY;
    
    uint32_t m_Id;
};

}; // namespace jam

#endif /* CCAMERA2D_H */
