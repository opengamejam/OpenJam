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
#include "IFrameBuffer.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CCamera2DPtr CCamera2D::Create(float _near, float _far)
{
    CCamera2DPtr camera(new CCamera2D(_near, _far));

    CTransformationComponentPtr transformComponent(new CTransformationComponent());

    CTransform3Df transform = transformComponent->Transform(CTransformationComponent::Local);
    transform.Position(glm::vec3(0.0f, 0.0f, 0.0f));
    transformComponent->AddTransform(CTransformationComponent::Local, transform);

    camera->Initialize("camera2d", { transformComponent });

    return camera;
}

CCamera2D::CCamera2D(float _near, float _far)
    : ICamera()
    , m_Near(_near)
    , m_Far(_far)
    , m_ProjectionMatrix(glm::mat4x4(1.0f))
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
    Get<CTransformationComponent>([&](CTransformationComponentPtr transformComponent) {
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
    m_ProjectionMatrix = glm::ortho(0.0f,
                                    static_cast<float>(renderTarget->Width()),
                                    static_cast<float>(renderTarget->Height()),
                                    0.0f, m_Near, m_Far);
}

void CCamera2D::FlipY()
{
    assert(RenderTarget());
    
    m_IsFlippedY = !m_IsFlippedY;
    float top = (m_IsFlippedY) ? RenderTarget()->Height() : 0;
    float bottom = (m_IsFlippedY) ? 0 : RenderTarget()->Height();

    m_ProjectionMatrix = glm::ortho(0.0f, static_cast<float>(RenderTarget()->Width()), bottom, top, m_Near, m_Far);
}

void CCamera2D::FlipX()
{
    assert(RenderTarget());
    
    m_IsFlippedX = !m_IsFlippedX;
    float left = (m_IsFlippedX) ? RenderTarget()->Width() : 0;
    float right = (m_IsFlippedX) ? 0 : RenderTarget()->Width();

    m_ProjectionMatrix = glm::ortho(left, right, static_cast<float>(RenderTarget()->Height()), 0.0f, m_Near, m_Far);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
