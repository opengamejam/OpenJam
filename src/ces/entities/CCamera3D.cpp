//
//  CCamera3D.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CCamera3D.h"
#include "CTransformationComponent.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CCamera3DPtr CCamera3D::Create(float fov, float width, float height, float near, float far)
{
    CCamera3DPtr camera(new CCamera3D(fov, width, height, near, far));
    
    CTransformationComponentPtr transformComponent(new CTransformationComponent());
    
    CTransform3Df transform = transformComponent->Transform(CTransformationComponent::Local);
    transform.Position(CVector3Df(0.0f, 0.0f, -10.0f));
    transformComponent->AddTransform(CTransformationComponent::Local, transform);
    
    transformComponent->ResultTransform(transform);
    
    camera->Initialize("camera3d", {transformComponent});
    
    return camera;
}

CCamera3D::CCamera3D(float fov, float width, float height, float near, float far)
: ICamera()
, m_Width(width)
, m_Height(height)
, m_Near(near)
, m_Far(far)
, m_RenderTarget(nullptr)
, m_Id(ICamera::NextCameraId())
{
    m_Near = std::max(0.01f, m_Near);
    m_ProjectionMatrix = CMatrix4x4f::Perspective(fov, m_Width / m_Height, m_Near, m_Far);
}

CCamera3D::~CCamera3D()
{

}

unsigned int CCamera3D::Id() const
{
    return m_Id;
}

CMatrix4x4f CCamera3D::ProjectionMatrix()
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
