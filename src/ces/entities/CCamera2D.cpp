//
//  CCamera2D.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CCamera2D.h"
#include "CTransformationComponent.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CCamera2DPtr CCamera2D::Create(float width, float height, float near, float far)
{
    CCamera2DPtr camera(new CCamera2D(width, height, near, far));
    
    CTransformationComponentPtr transformComponent(new CTransformationComponent());
    
    CTransform3Df transform = transformComponent->Transform(CTransformationComponent::Local);
    transform.Position(CVector3Df(-0.5f * width, -0.5f * height));
    transformComponent->AddTransform(CTransformationComponent::Local, transform);
    
    camera->Initialize("camera", {transformComponent});
    
    return camera;
}

CCamera2D::CCamera2D(float width, float height, float near, float far)
: ICamera()
, m_Width(width)
, m_Height(height)
, m_Near(near)
, m_Far(far)
, m_RenderTarget(nullptr)
, m_IsFlippedX(false)
, m_IsFlippedY(false)
{
    m_ProjectionMatrix = CMatrix4x4f::Orthographic(0, m_Width,
                                                   0, m_Height,
                                                   m_Near, m_Far);
}

CCamera2D::~CCamera2D()
{

}

CMatrix4x4f CCamera2D::ProjectionMatrix()
{
    CTransform3Df resultTransform;
    Get<CTransformationComponent>([&](CTransformationComponentPtr transformComponent)
    {
        resultTransform = transformComponent->ResultTransform();
    });
    
    return m_ProjectionMatrix * resultTransform();
}

IRenderTargetPtr CCamera2D::RenderTarget() const
{
    return m_RenderTarget;
}

void CCamera2D::RenderTarget(IRenderTargetPtr renderTarget)
{
    m_RenderTarget = renderTarget;
}

void CCamera2D::FlipY()
{
    m_IsFlippedY = !m_IsFlippedY;
    float top = (m_IsFlippedY) ? m_Height : 0;
    float bottom = (m_IsFlippedY) ? 0 : m_Height;
    
    m_ProjectionMatrix = CMatrix4x4f::Orthographic(0, m_Width,
                                                   top, bottom,
                                                   m_Near, m_Far);
}

void CCamera2D::FlipX()
{
    m_IsFlippedX = !m_IsFlippedX;
    float left = (m_IsFlippedX) ? m_Width : 0;
    float right = (m_IsFlippedX) ? 0 : m_Width;
    
    m_ProjectionMatrix = CMatrix4x4f::Orthographic(left, right,
                                                   0, m_Height,
                                                   m_Near, m_Far);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
