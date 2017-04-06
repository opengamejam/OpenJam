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
    , m_ProjectionMatrix()
    , m_FrameBuffer()
    , m_IsFlippedX(false)
    , m_IsFlippedY(false)
{
}

CCamera2D::~CCamera2D()
{
}

glm::mat4x4 CCamera2D::ProjectionMatrix(uint64_t frameBufferIndex)
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
    return m_ProjectionMatrix[frameBufferIndex] * resultTransform();
}

IFrameBufferPtr CCamera2D::FrameBuffer(uint64_t frameBufferIndex) const
{
    frameBufferIndex = std::min<uint64_t>(frameBufferIndex, m_FrameBuffer.size());
    if (frameBufferIndex >= m_FrameBuffer.size()) {
        return nullptr;
    }
    return m_FrameBuffer[frameBufferIndex];
}

void CCamera2D::FrameBuffer(IFrameBufferPtr frameBuffer, uint64_t frameBufferIndex)
{
    assert("Framebuffer's dimensions cannot be zero" && frameBuffer->Width() > 0 &&  frameBuffer->Height() > 0);
    
    if (!frameBuffer) {
        if (frameBufferIndex < m_FrameBuffer.size()) {
            m_FrameBuffer.erase(m_FrameBuffer.begin() + frameBufferIndex);
            m_ProjectionMatrix.erase(m_ProjectionMatrix.begin() + frameBufferIndex);
        }
        return;
    }
    
    glm::mat4 ortho = Ortho(frameBuffer->Width(), frameBuffer->Height(), m_IsFlippedX, m_IsFlippedY);
    frameBufferIndex = std::min<uint64_t>(frameBufferIndex, m_FrameBuffer.size());
    if (frameBufferIndex >= m_FrameBuffer.size()) {
        m_FrameBuffer.push_back(frameBuffer);
        m_ProjectionMatrix.push_back(ortho);
    } else {
        m_FrameBuffer[frameBufferIndex] = frameBuffer;
        m_ProjectionMatrix[frameBufferIndex] = ortho;
    }
}

uint64_t CCamera2D::FrameBufferCount() const
{
    return m_FrameBuffer.size();
}

void CCamera2D::FlipY()
{
    m_IsFlippedY = !m_IsFlippedY;
    for (uint64_t i = 0; i < m_FrameBuffer.size(); ++i) {
        m_ProjectionMatrix[i] = Ortho(FrameBuffer()->Width(), FrameBuffer()->Height(), m_IsFlippedX, m_IsFlippedY);
    }
}

void CCamera2D::FlipX()
{
    m_IsFlippedX = !m_IsFlippedX;
    for (uint64_t i = 0; i < m_FrameBuffer.size(); ++i) {
        m_ProjectionMatrix[i] = Ortho(FrameBuffer()->Width(), FrameBuffer()->Height(), m_IsFlippedX, m_IsFlippedY);
    }
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

glm::mat4 CCamera2D::Ortho(uint32_t w, uint32_t h, bool flipX, bool flipY)
{
    float left = (flipX) ? w : 0;
    float right = (flipX) ? 0 : w;
    
    float top = (flipY) ? h : 0;
    float bottom = (flipY) ? 0 : h;
    
    return glm::ortho(left, right, bottom, top, m_Near, m_Far);
}
