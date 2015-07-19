//
//  CVertexBufferOGLES20.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#include "CVertexBufferOGLES20.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CVertexBufferOGLES20::CVertexBufferOGLES20()
: m_Id(0)
, m_ElementSize(0)
, m_IsLocked(false)
{
    
}

CVertexBufferOGLES20::~CVertexBufferOGLES20()
{
    Destroy();
}

void CVertexBufferOGLES20::Initialize(size_t elementSize)
{
    if (!IsValid())
    {
        glGenBuffers(1, &m_Id);
    }
    ElementSize(elementSize);
}

const IVertexBuffer::TVertexStreamMap& CVertexBufferOGLES20::VertexStreams() const
{
    return m_VertexStreamers;
}

IVertexBuffer::SVertexStream& CVertexBufferOGLES20::Lock(IVertexBuffer::VertexTypes vertexType)
{
    LockRaw();
    
    if (m_VertexStreamers.find(vertexType) == m_VertexStreamers.end())
    {
        m_VertexStreamers[vertexType] = SVertexStream(shared_from_this());
    }
    
    return m_VertexStreamers[vertexType];
}

void CVertexBufferOGLES20::Destroy()
{
    if (IsValid())
    {
        glDeleteBuffers(1, &m_Id);
        m_Id = 0;
    }
}

bool CVertexBufferOGLES20::IsValid() const
{
    return (m_Id != 0);
}

size_t CVertexBufferOGLES20::SizeRaw() const
{
    return m_Buffer.size();
}

void CVertexBufferOGLES20::ResizeRaw(size_t newSize)
{
    m_Buffer.resize(newSize);
}

size_t CVertexBufferOGLES20::ElementSize() const
{
    return m_ElementSize;
}

void* CVertexBufferOGLES20::LockRaw()
{
    m_IsLocked = true;
    return m_Buffer.data();
}

bool CVertexBufferOGLES20::IsLocked() const
{
    return m_IsLocked;
}

void CVertexBufferOGLES20::Unlock()
{
    if (!m_IsLocked)
    {
        return;
    }
    
    Bind();
    glBufferData(GL_ARRAY_BUFFER, m_Buffer.size(), m_Buffer.data(), GL_DYNAMIC_DRAW);
    Unbind();
    m_IsLocked = false;
}

void CVertexBufferOGLES20::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_Id);
    
    const IVertexBuffer::TVertexStreamMap& vertexStreams = VertexStreams();
    std::for_each(vertexStreams.begin(), vertexStreams.end(),
                  [&](const IVertexBuffer::TVertexStreamMap::value_type& value)
    {
        if (value.second.IsActive())
        {
            GLbyte *ptr = nullptr;
            glEnableVertexAttribArray(value.second.binding);
            glVertexAttribPointer(value.second.binding,
                                  value.second.stride,
                                  GL_FLOAT, // TODO: type
                                  GL_FALSE,
                                  (GLsizei)ElementSize(),
                                  (GLvoid*)(ptr + value.second.offset));
        }
    });
}

bool CVertexBufferOGLES20::HasStream(IVertexBuffer::VertexTypes vertexType)
{
    return (m_VertexStreamers.find(vertexType) != m_VertexStreamers.end());
}

void CVertexBufferOGLES20::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CVertexBufferOGLES20::ElementSize(size_t elementSize)
{
    m_ElementSize = std::max<size_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGLES2