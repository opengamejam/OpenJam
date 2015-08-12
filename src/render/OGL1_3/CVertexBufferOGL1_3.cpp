//
//  CVertexBufferOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#include "CVertexBufferOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CVertexBufferOGL1_3::CVertexBufferOGL1_3()
: m_Id(1)
, m_ElementSize(0)
, m_IsLocked(false)
{
    
}

CVertexBufferOGL1_3::~CVertexBufferOGL1_3()
{
    
}

void CVertexBufferOGL1_3::Initialize(size_t elementSize)
{
    ElementSize(elementSize);
}

const IVertexBuffer::TVertexStreamMap& CVertexBufferOGL1_3::VertexStreams() const
{
    return m_VertexStreamers;
}

IVertexBuffer::SVertexStream& CVertexBufferOGL1_3::Lock(IVertexBuffer::VertexTypes vertexType)
{
    LockRaw();
    
    if (m_VertexStreamers.find(vertexType) == m_VertexStreamers.end())
    {
        m_VertexStreamers[vertexType] = SVertexStream(shared_from_this());
    }
    
    return m_VertexStreamers[vertexType];
}

void CVertexBufferOGL1_3::Destroy()
{
    
}

bool CVertexBufferOGL1_3::IsValid() const
{
    return (m_Id != 0);
}

size_t CVertexBufferOGL1_3::SizeRaw() const
{
    return m_Buffer.size();
}

void CVertexBufferOGL1_3::ResizeRaw(size_t newSize)
{
    m_Buffer.resize(newSize);
}

size_t CVertexBufferOGL1_3::ElementSize() const
{
    return m_ElementSize;
}

void* CVertexBufferOGL1_3::LockRaw()
{
    m_IsLocked = true;
    return m_Buffer.data();
}

bool CVertexBufferOGL1_3::IsLocked() const
{
    return m_IsLocked;
}

void CVertexBufferOGL1_3::Unlock()
{
    if (!m_IsLocked)
    {
        return;
    }
    
    m_IsLocked = false;
}

bool CVertexBufferOGL1_3::HasStream(IVertexBuffer::VertexTypes vertexType)
{
    return (m_VertexStreamers.find(vertexType) != m_VertexStreamers.end());
}

void CVertexBufferOGL1_3::Bind()
{
    
}

void CVertexBufferOGL1_3::Unbind()
{
    
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CVertexBufferOGL1_3::ElementSize(size_t elementSize)
{
    m_ElementSize = std::max<size_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL1_3