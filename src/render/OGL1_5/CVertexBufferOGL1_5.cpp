//
//  CVertexBufferOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#include "CVertexBufferOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CVertexBufferOGL1_5::CVertexBufferOGL1_5()
: m_Id(0)
, m_ElementSize(0)
, m_IsLocked(false)
{
    
}

CVertexBufferOGL1_5::~CVertexBufferOGL1_5()
{
    Destroy();
}

void CVertexBufferOGL1_5::Initialize(size_t elementSize)
{
    if (!IsValid())
    {
        glGenBuffers(1, &m_Id);
    }
    ElementSize(elementSize);
}

const IVertexBuffer::TVertexStreamMap& CVertexBufferOGL1_5::VertexStreams() const
{
    return m_VertexStreamers;
}

IVertexBuffer::SVertexStream& CVertexBufferOGL1_5::Lock(IVertexBuffer::VertexTypes vertexType)
{
    LockRaw();
    
    if (m_VertexStreamers.find(vertexType) == m_VertexStreamers.end())
    {
        m_VertexStreamers[vertexType] = SVertexStream(shared_from_this());
    }
    
    return m_VertexStreamers[vertexType];
}

void CVertexBufferOGL1_5::Destroy()
{
    if (IsValid())
    {
        glDeleteBuffers(1, &m_Id);
        m_Id = 0;
    }
}

bool CVertexBufferOGL1_5::IsValid() const
{
    return (m_Id != 0);
}

size_t CVertexBufferOGL1_5::SizeRaw() const
{
    return m_Buffer.size();
}

void CVertexBufferOGL1_5::ResizeRaw(size_t newSize)
{
    m_Buffer.resize(newSize);
}

size_t CVertexBufferOGL1_5::ElementSize() const
{
    return m_ElementSize;
}

void* CVertexBufferOGL1_5::LockRaw()
{
    m_IsLocked = true;
    return m_Buffer.data();
}

bool CVertexBufferOGL1_5::IsLocked() const
{
    return m_IsLocked;
}

void CVertexBufferOGL1_5::Unlock()
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

bool CVertexBufferOGL1_5::HasStream(IVertexBuffer::VertexTypes vertexType)
{
    return (m_VertexStreamers.find(vertexType) != m_VertexStreamers.end());
}

void CVertexBufferOGL1_5::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_Id);
    
    const IVertexBuffer::TVertexStreamMap& vertexStreams = VertexStreams();
    std::for_each(vertexStreams.begin(), vertexStreams.end(),
                  [&](const IVertexBuffer::TVertexStreamMap::value_type& value)
    {
        const SVertexStream& stream = value.second;
        if (stream.IsActive())
        {
            GLbyte *offset = nullptr;
            offset += value.second.offset;
            int type = ConvertDataType(stream.dataType);
            
            if (value.first == IVertexBuffer::Position)
            {
                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(stream.stride, type, (GLsizei)ElementSize(), (GLvoid*)offset);
            }
            else if (value.first == IVertexBuffer::TextureCoors)
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(stream.stride, type, (GLsizei)ElementSize(), (GLvoid*)offset);
            }
            else if (value.first == IVertexBuffer::Color)
            {
                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(stream.stride, type, (GLsizei)ElementSize(), (GLvoid*)offset);
            }
            else if (value.first == IVertexBuffer::Normal)
            {
                glEnableClientState(GL_NORMAL_ARRAY);
                glNormalPointer(type, (GLsizei)ElementSize(), (GLvoid*)offset);
            }
            else if (value.first == IVertexBuffer::Tangent)
            {
            }
        }
    });
}

void CVertexBufferOGL1_5::Unbind()
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

void CVertexBufferOGL1_5::ElementSize(size_t elementSize)
{
    m_ElementSize = std::max<size_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

int CVertexBufferOGL1_5::ConvertDataType(DataTypes dataType)
{
    static std::map<DataTypes, int> converter = {
        {Unknown, GL_BYTE},
        {Byte, GL_BYTE},
        {UByte, GL_UNSIGNED_BYTE},
        {Short, GL_SHORT},
        {UShort, GL_UNSIGNED_SHORT},
        {Int, GL_INT},
        {UInt, GL_UNSIGNED_INT},
        {Float, GL_FLOAT},
        {ShortFloat, GL_2_BYTES}, // TODO
    };
    
    return converter[dataType];
}

#endif // RENDER_OGL1_5 || RENDER_OGLES1_1