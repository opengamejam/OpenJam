//
//  CVertexBufferOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CVertexBufferOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CVertexBufferOGL2_0::CVertexBufferOGL2_0()
: m_Id(0)
, m_ElementSize(0)
, m_IsLocked(false)
{
    
}

CVertexBufferOGL2_0::~CVertexBufferOGL2_0()
{
    Destroy();
}

void CVertexBufferOGL2_0::Initialize(size_t elementSize)
{
    if (!IsValid())
    {
        glGenBuffers(1, &m_Id);
    }
    ElementSize(elementSize);
}

const IVertexBuffer::TVertexStreamMap& CVertexBufferOGL2_0::VertexStreams() const
{
    return m_VertexStreamers;
}

IVertexBuffer::SVertexStream& CVertexBufferOGL2_0::Lock(IVertexBuffer::VertexTypes vertexType)
{
    LockRaw();
    
    if (m_VertexStreamers.find(vertexType) == m_VertexStreamers.end())
    {
        m_VertexStreamers[vertexType] = SVertexStream(shared_from_this());
    }
    
    return m_VertexStreamers[vertexType];
}

void CVertexBufferOGL2_0::Destroy()
{
    if (IsValid())
    {
        glDeleteBuffers(1, &m_Id);
        m_Id = 0;
    }
}

bool CVertexBufferOGL2_0::IsValid() const
{
    return (m_Id != 0);
}

size_t CVertexBufferOGL2_0::SizeRaw() const
{
    return m_Buffer.size();
}

void CVertexBufferOGL2_0::ResizeRaw(size_t newSize)
{
    m_Buffer.resize(newSize);
}

size_t CVertexBufferOGL2_0::ElementSize() const
{
    return m_ElementSize;
}

void* CVertexBufferOGL2_0::LockRaw()
{
    m_IsLocked = true;
    return m_Buffer.data();
}

bool CVertexBufferOGL2_0::IsLocked() const
{
    return m_IsLocked;
}

void CVertexBufferOGL2_0::Unlock()
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

void CVertexBufferOGL2_0::Bind()
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

bool CVertexBufferOGL2_0::HasStream(IVertexBuffer::VertexTypes vertexType)
{
    return (m_VertexStreamers.find(vertexType) != m_VertexStreamers.end());
}

void CVertexBufferOGL2_0::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CVertexBufferOGL2_0::ElementSize(size_t elementSize)
{
    m_ElementSize = std::max<size_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0