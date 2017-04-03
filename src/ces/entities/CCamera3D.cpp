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
#include "IFrameBuffer.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CCamera3DPtr CCamera3D::Create(float _fov, float _near, float _far)
{
    CCamera3DPtr camera(new CCamera3D(_fov, _near, _far));

    CTransformationComponentPtr transformComponent(new CTransformationComponent());

    CTransform3Df transform = transformComponent->Transform(CTransformationComponent::Local);
    transform.Position(glm::vec3(0.0f, 0.0f, -10.0f));
    transformComponent->AddTransform(CTransformationComponent::Local, transform);

    camera->Initialize("camera3d", { transformComponent });

    return camera;
}

CCamera3D::CCamera3D(float _fov, float _near, float _far)
    : ICamera()
    , m_FOV(_fov)
    , m_Near(std::max(0.01f, _near))
    , m_Far(_far)
    , m_ProjectionMatrix(glm::mat4x4(1.0f))
    , m_FrameBuffer(nullptr)
{
}

CCamera3D::~CCamera3D()
{
}

glm::mat4x4 CCamera3D::ProjectionMatrix()
{
    CTransform3Df resultTransform;
    Get<CTransformationComponent>([&](CTransformationComponentPtr transformComponent) {
        transformComponent->UpdateAbsoluteTransform();
        resultTransform = transformComponent->AbsoluteTransformation();
    });

    return m_ProjectionMatrix * resultTransform();
}

IFrameBufferPtr CCamera3D::FrameBuffer() const
{
    return m_FrameBuffer;
}

void CCamera3D::FrameBuffer(IFrameBufferPtr frameBuffer)
{
    m_FrameBuffer = frameBuffer;
    float koeff = static_cast<float>(frameBuffer->Width()) / static_cast<float>(frameBuffer->Height());
    m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV),
                                          koeff,
                                          m_Near, m_Far);
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
