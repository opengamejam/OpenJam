//
//  CMeshOGLES2.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#include "CMeshOGLES2.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMeshOGLES2::CMeshOGLES2()
: m_VAO(0)
, m_VertexBuffer(nullptr)
, m_IndexBuffer(nullptr)
{
    glGenVertexArraysOES(1, &m_VAO);
}

CMeshOGLES2::~CMeshOGLES2()
{
    glDeleteVertexArraysOES(1, &m_VAO);
}

void CMeshOGLES2::Bind()
{
    glBindVertexArrayOES(m_VAO);
}

void CMeshOGLES2::Unbind()
{
    glBindVertexArrayOES(0);
}

bool CMeshOGLES2::IsValid() const
{
    return (m_VAO != 0);
}

IVertexBufferPtr CMeshOGLES2::VertexBuffer() const
{
    return m_VertexBuffer;
}

void CMeshOGLES2::VertexBuffer(IVertexBufferPtr vertexBuffer)
{
    if (m_VertexBuffer != vertexBuffer)
    {
        Bind();
        
        if (m_VertexBuffer)
        {
            m_VertexBuffer->Unbind();
        }
        if (vertexBuffer)
        {
            vertexBuffer->Bind();
        }
        
        Unbind();
    }
    
    m_VertexBuffer = vertexBuffer;
}

IIndexBufferPtr CMeshOGLES2::IndexBuffer() const
{
    return m_IndexBuffer;
}

void CMeshOGLES2::IndexBuffer(IIndexBufferPtr indexBuffer)
{
    if (m_IndexBuffer != indexBuffer)
    {
        Bind();
        
        if (m_IndexBuffer)
        {
            m_IndexBuffer->Unbind();
        }
        if (indexBuffer)
        {
            indexBuffer->Bind();
        }
        
        Unbind();
    }
    
    m_IndexBuffer = indexBuffer;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGLES2