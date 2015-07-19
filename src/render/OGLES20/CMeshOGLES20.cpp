//
//  CMeshOGLES20.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#include "CMeshOGLES20.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMeshOGLES20::CMeshOGLES20()
: m_VAO(0)
, m_VertexBuffer(nullptr)
, m_IndexBuffer(nullptr)
{
    glGenVertexArraysOES(1, &m_VAO);
}

CMeshOGLES20::~CMeshOGLES20()
{
    glDeleteVertexArraysOES(1, &m_VAO);
}

void CMeshOGLES20::Bind()
{
    glBindVertexArrayOES(m_VAO);
}

void CMeshOGLES20::Unbind()
{
    glBindVertexArrayOES(0);
}

bool CMeshOGLES20::IsValid() const
{
    return (m_VAO != 0);
}

IVertexBufferPtr CMeshOGLES20::VertexBuffer() const
{
    return m_VertexBuffer;
}

void CMeshOGLES20::VertexBuffer(IVertexBufferPtr vertexBuffer)
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

IIndexBufferPtr CMeshOGLES20::IndexBuffer() const
{
    return m_IndexBuffer;
}

void CMeshOGLES20::IndexBuffer(IIndexBufferPtr indexBuffer)
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