//
//  CCamera2D.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CCamera2D.h"
#include "CTransformationComponent.h"
#include "glm/ext.hpp"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CCamera2DPtr CCamera2D::Create(float _width, float _height, float _near, float _far)
{
    CCamera2DPtr camera(new CCamera2D(_width, _height, _near, _far));
    
    CTransformationComponentPtr transformComponent(new CTransformationComponent());
    
    CTransform3Df transform = transformComponent->Transform(CTransformationComponent::Local);
    transform.Position(glm::vec3(0.0f, 0.0f, 0.0f));
    transformComponent->AddTransform(CTransformationComponent::Local, transform);
    
    camera->Initialize("camera2d", {transformComponent});
    
    return camera;
}

CCamera2D::CCamera2D(float _width, float _height, float _near, float _far)
: ICamera()
, m_Width(_width)
, m_Height(_height)
, m_Near(_near)
, m_Far(_far)
, m_ProjectionMatrix(glm::ortho(0.0f, m_Width, m_Height, 0.0f, m_Near, m_Far))
, m_RenderTarget(nullptr)
, m_IsFlippedX(false)
, m_IsFlippedY(false)
, m_Id(ICamera::NextCameraId())
{
}

CCamera2D::~CCamera2D()
{

}

uint32_t CCamera2D::Id() const
{
    return m_Id;
}

glm::mat4x4 CCamera2D::ProjectionMatrix()
{
    CTransform3Df resultTransform;
    Get<CTransformationComponent>([&](CTransformationComponentPtr transformComponent)
    {
        resultTransform = transformComponent->ResultTransform();
    });
    
    return m_ProjectionMatrix * resultTransform();
}

IFrameBufferPtr CCamera2D::RenderTarget() const
{
    return m_RenderTarget;
}

void CCamera2D::RenderTarget(IFrameBufferPtr renderTarget)
{
    m_RenderTarget = renderTarget;
}

void CCamera2D::FlipY()
{
    m_IsFlippedY = !m_IsFlippedY;
    float top = (m_IsFlippedY) ? m_Height : 0;
    float bottom = (m_IsFlippedY) ? 0 : m_Height;
    
    m_ProjectionMatrix = glm::ortho(0.0f, m_Width, bottom, top, m_Near, m_Far);
}

void CCamera2D::FlipX()
{
    m_IsFlippedX = !m_IsFlippedX;
    float left = (m_IsFlippedX) ? m_Width : 0;
    float right = (m_IsFlippedX) ? 0 : m_Width;
    
    m_ProjectionMatrix = glm::ortho(left, right, m_Height, 0.0f, m_Near, m_Far);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
