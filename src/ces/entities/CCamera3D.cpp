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
    , m_ProjectionMatrix()
    , m_FrameBuffer()
{
}

CCamera3D::~CCamera3D()
{
}

glm::mat4x4 CCamera3D::ProjectionMatrix(uint64_t frameBufferIndex)
{
    CTransform3Df resultTransform;
    Get<CTransformationComponent>([&](CTransformationComponentPtr transformComponent) {
        transformComponent->UpdateAbsoluteTransform();
        resultTransform = transformComponent->AbsoluteTransformation();
    });
    
    frameBufferIndex = std::min<uint64_t>(frameBufferIndex, m_FrameBuffer.size());
    if (frameBufferIndex >= m_FrameBuffer.size()) {
        return glm::mat4(1.0);
    }
    return m_ProjectionMatrix[(size_t)frameBufferIndex] * resultTransform();
}

IFrameBufferPtr CCamera3D::FrameBuffer(uint64_t frameBufferIndex) const
{
    frameBufferIndex = std::min<uint64_t>(frameBufferIndex, m_FrameBuffer.size());
    if (frameBufferIndex >= m_FrameBuffer.size()) {
        return nullptr;
    }
    return m_FrameBuffer[(size_t)frameBufferIndex];
}

void CCamera3D::FrameBuffer(IFrameBufferPtr frameBuffer, uint64_t frameBufferIndex)
{
    assert("Framebuffer's dimensions cannot be zero" && frameBuffer->Width() > 0 &&  frameBuffer->Height() > 0);
    
    if (!frameBuffer) {
        if (frameBufferIndex < m_FrameBuffer.size()) {
            m_FrameBuffer.erase(m_FrameBuffer.begin() + (size_t)frameBufferIndex);
            m_ProjectionMatrix.erase(m_ProjectionMatrix.begin() + (size_t)frameBufferIndex);
        }
        return;
    }
    
    float koeff = static_cast<float>(frameBuffer->Width()) / static_cast<float>(frameBuffer->Height());
    glm::mat4 perspective = glm::perspective(glm::radians(m_FOV),
                                             koeff,
                                             m_Near, m_Far);
    frameBufferIndex = std::min<uint64_t>(frameBufferIndex, m_FrameBuffer.size());
    if (frameBufferIndex >= m_FrameBuffer.size()) {
        m_FrameBuffer.push_back(frameBuffer);
        m_ProjectionMatrix.push_back(perspective);
    } else {
        m_FrameBuffer[(size_t)frameBufferIndex] = frameBuffer;
        m_ProjectionMatrix[(size_t)frameBufferIndex] = perspective;
    }
}

uint64_t CCamera3D::FrameBufferCount() const
{
    return m_FrameBuffer.size();
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
