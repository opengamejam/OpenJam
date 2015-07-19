//
//  CCamera2D.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CCamera2D.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

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
    Offset(CVector3Df(-0.5f * m_Width, -0.5f * m_Height));
}

CCamera2D::~CCamera2D()
{

}

CMatrix4x4f CCamera2D::ProjectionMatrix()
{    
    CTransform3Df transform = WorldTransform();
    
    IGameObjectPtr parent = Parent().lock();
    if (parent)
    {
        CTransform3Df externalTransform = parent->Transform();

        transform.Offset(CVector3Df(-externalTransform.Position().X(),
                                    -externalTransform.Position().Y(),
                                    -externalTransform.Position().Z()));
        transform.Position(CVector3Df(-Offset().X(),
                                      -Offset().Y(),
                                      -Offset().Z()));
    }
    
    return m_ProjectionMatrix * transform();
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
