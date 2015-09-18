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
, m_ZeroStride(false)
{
    
}

CVertexBufferOGL2_0::~CVertexBufferOGL2_0()
{
    Destroy();
}

void CVertexBufferOGL2_0::Initialize(uint64_t elementSize)
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
        unsigned int absoluteOffset = 0;
        std::for_each(m_VertexStreamers.begin(), m_VertexStreamers.end(), [&](const TVertexStreamMap::value_type& value)
                      {
                          const IVertexBuffer::SVertexStream& stream = value.second;
                          absoluteOffset += (stream.DataSize() * stream.stride * Size());
                      });
        
        SVertexStream stream = SVertexStream(shared_from_this());
        stream.streamIndex = (unsigned int)m_VertexStreamers.size();
        stream.absoluteOffset = absoluteOffset;
        
        m_VertexStreamers[vertexType] = stream;
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

uint64_t CVertexBufferOGL2_0::SizeRaw() const
{
    return m_Buffer.size();
}

void CVertexBufferOGL2_0::ResizeRaw(uint64_t newSize)
{
    m_Buffer.resize(newSize);
}

uint64_t CVertexBufferOGL2_0::ElementSize() const
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

bool CVertexBufferOGL2_0::HasStream(IVertexBuffer::VertexTypes vertexType)
{
    return (m_VertexStreamers.find(vertexType) != m_VertexStreamers.end());
}

void CVertexBufferOGL2_0::ZeroStride(bool isZeroStride)
{
    m_ZeroStride = isZeroStride;
}

bool CVertexBufferOGL2_0::ZeroStride()
{
    return m_ZeroStride;
}

void CVertexBufferOGL2_0::Bind()
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
            offset += (ZeroStride() ? stream.absoluteOffset : stream.offset);
            int type = ConvertDataType(stream.dataType);
            GLsizei elementSize = (ZeroStride() ? 0 : (GLsizei)ElementSize());
            
            glEnableVertexAttribArray(stream.attributeIndex);
            glVertexAttribPointer(stream.attributeIndex,
                                  stream.stride,
                                  type,
                                  GL_FALSE,
                                  elementSize,
                                  (GLvoid*)offset);
        }
    });
}

void CVertexBufferOGL2_0::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CVertexBufferOGL2_0::ElementSize(uint64_t elementSize)
{
    m_ElementSize = std::max<uint64_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

int CVertexBufferOGL2_0::ConvertDataType(DataTypes dataType)
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
        //{ShortFloat, GL_2_BYTES}, // TODO
    };
    
    return converter[dataType];
}

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0