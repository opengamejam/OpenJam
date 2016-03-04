//
//  CCamera3D.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CCamera3D.h"
#include "CTransformationComponent.h"
#include "glm/ext.hpp"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CCamera3DPtr CCamera3D::Create(float _fov, float _width, float _height, float _near, float _far)
{
    CCamera3DPtr camera(new CCamera3D(_fov, _width, _height, _near, _far));
    
    CTransformationComponentPtr transformComponent(new CTransformationComponent());
    
    CTransform3Df transform = transformComponent->Transform(CTransformationComponent::Local);
    transform.Position(glm::vec3(0.0f, 0.0f, -10.0f));
    transformComponent->AddTransform(CTransformationComponent::Local, transform);
    
    transformComponent->ResultTransform(transform);
    
    camera->Initialize("camera3d", {transformComponent});
    
    return camera;
}

CCamera3D::CCamera3D(float _fov, float _width, float _height, float _near, float _far)
: ICamera()
, m_Width(_width)
, m_Height(_height)
, m_Near(std::max(0.01f, _near))
, m_Far(_far)
, m_ProjectionMatrix(glm::perspective(glm::radians(_fov), m_Width / m_Height, m_Near, m_Far))
, m_RenderTarget(nullptr)
, m_Id(ICamera::NextCameraId())
{
    
}

CCamera3D::~CCamera3D()
{

}

uint32_t CCamera3D::Id() const
{
    return m_Id;
}

glm::mat4x4 CCamera3D::ProjectionMatrix()
{
    CTransform3Df resultTransform;
    Get<CTransformationComponent>([&](CTransformationComponentPtr transformComponent)
    {
        resultTransform = transformComponent->ResultTransform();
    });
    
    return m_ProjectionMatrix * resultTransform();
}

IRenderTargetPtr CCamera3D::RenderTarget() const
{
    return m_RenderTarget;
}

void CCamera3D::RenderTarget(IRenderTargetPtr renderTarget)
{
    m_RenderTarget = renderTarget;
}

void CCamera3D::FlipY()
{
    // TODO:
}

void CCamera3D::FlipX()
{
    // TODO:
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
