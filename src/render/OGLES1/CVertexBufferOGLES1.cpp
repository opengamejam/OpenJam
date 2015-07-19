//
//  CVertexBufferOGLES1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1)

#include "CVertexBufferOGLES1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CVertexBufferOGLES1::CVertexBufferOGLES1()
: m_Id(0)
, m_ElementSize(0)
, m_IsLocked(false)
{
    
}

CVertexBufferOGLES1::~CVertexBufferOGLES1()
{
    Destroy();
}

void CVertexBufferOGLES1::Initialize(size_t elementSize)
{
    if (!IsValid())
    {
        glGenBuffers(1, &m_Id);
    }
    ElementSize(elementSize);
}

const IVertexBuffer::TVertexStreamMap& CVertexBufferOGLES1::VertexStreams() const
{
    return m_VertexStreamers;
}

IVertexBuffer::SVertexStream& CVertexBufferOGLES1::Lock(IVertexBuffer::VertexTypes vertexType)
{
    LockRaw();
    
    if (m_VertexStreamers.find(vertexType) == m_VertexStreamers.end())
    {
        m_VertexStreamers[vertexType] = SVertexStream(shared_from_this());
    }
    
    return m_VertexStreamers[vertexType];
}

void CVertexBufferOGLES1::Destroy()
{
    if (IsValid())
    {
        glDeleteBuffers(1, &m_Id);
        m_Id = 0;
    }
}

bool CVertexBufferOGLES1::IsValid() const
{
    return (m_Id != 0);
}

size_t CVertexBufferOGLES1::SizeRaw() const
{
    return m_Buffer.size();
}

void CVertexBufferOGLES1::ResizeRaw(size_t newSize)
{
    m_Buffer.resize(newSize);
}

size_t CVertexBufferOGLES1::ElementSize() const
{
    return m_ElementSize;
}

void* CVertexBufferOGLES1::LockRaw()
{
    m_IsLocked = true;
    return m_Buffer.data();
}

bool CVertexBufferOGLES1::IsLocked() const
{
    return m_IsLocked;
}

void CVertexBufferOGLES1::Unlock()
{
    if (!m_IsLocked)
    {
        return;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, m_Id);
    glBufferData(GL_ARRAY_BUFFER, m_Buffer.size(), m_Buffer.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    m_IsLocked = false;
}

bool CVertexBufferOGLES1::HasStream(IVertexBuffer::VertexTypes vertexType)
{
    return (m_VertexStreamers.find(vertexType) != m_VertexStreamers.end());
}

void CVertexBufferOGLES1::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_Id);
    
    const IVertexBuffer::TVertexStreamMap& vertexStreams = VertexStreams();
    std::for_each(vertexStreams.begin(), vertexStreams.end(),
                  [&](const IVertexBuffer::TVertexStreamMap::value_type& value)
    {
        if (value.second.IsActive())
        {
            GLbyte *offset = nullptr;
            offset += value.second.offset;
            if (value.first == IVertexBuffer::Position)
            {
                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(value.second.stride,
                                GL_FLOAT,   // TODO: parse type
                                (GLsizei)ElementSize(),
                                (GLvoid*)offset);
            }
            else if (value.first == IVertexBuffer::TextureCoors)
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(value.second.stride,
                                  GL_FLOAT,   // TODO: parse type
                                  (GLsizei)ElementSize(),
                                  (GLvoid*)offset);
            }
            else if (value.first == IVertexBuffer::Color)
            {
                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(value.second.stride,
                               GL_FLOAT,   // TODO: parse type
                               (GLsizei)ElementSize(),
                               (GLvoid*)offset);
            }
            else if (value.first == IVertexBuffer::Normal)
            {
                glEnableClientState(GL_NORMAL_ARRAY);
                glNormalPointer(GL_FLOAT,   // TODO: parse type
                                (GLsizei)ElementSize(),
                                (GLvoid*)offset);
            }
            else if (value.first == IVertexBuffer::Tangent)
            {
            }
        }
    });
}

void CVertexBufferOGLES1::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    const IVertexBuffer::TVertexStreamMap& vertexStreams = VertexStreams();
    std::for_each(vertexStreams.begin(), vertexStreams.end(),
                  [&](const IVertexBuffer::TVertexStreamMap::value_type& value)
    {
        if (value.second.IsActive())
        {
            if (value.first == IVertexBuffer::Position)
            {
                glDisableClientState(GL_VERTEX_ARRAY);
            }
            else if (value.first == IVertexBuffer::TextureCoors)
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            else if (value.first == IVertexBuffer::Color)
            {
                glDisableClientState(GL_COLOR_ARRAY);
            }
            else if (value.first == IVertexBuffer::Normal)
            {
                glDisableClientState(GL_NORMAL_ARRAY);
            }
            else if (value.first == IVertexBuffer::Tangent)
            {
            }
        }
    });
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CVertexBufferOGLES1::ElementSize(size_t elementSize)
{
    m_ElementSize = std::max<size_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGLES1