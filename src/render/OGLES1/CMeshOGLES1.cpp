//
//  CMeshOGLES1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1)

#include "CMeshOGLES1.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMeshOGLES1::CMeshOGLES1()
: m_VertexBuffer(nullptr)
, m_IndexBuffer(nullptr)
{
    
}

CMeshOGLES1::~CMeshOGLES1()
{
    
}

void CMeshOGLES1::Bind()
{
    if (VertexBuffer())
    {
        VertexBuffer()->Bind();
    }
    if (IndexBuffer())
    {
        IndexBuffer()->Bind();
    }
}

void CMeshOGLES1::Unbind()
{
    if (VertexBuffer())
    {
        VertexBuffer()->Unbind();
    }
    if (IndexBuffer())
    {
        IndexBuffer()->Unbind();
    }
}

bool CMeshOGLES1::IsValid() const
{
    return true;
}

IVertexBufferPtr CMeshOGLES1::VertexBuffer() const
{
    return m_VertexBuffer;
}

void CMeshOGLES1::VertexBuffer(IVertexBufferPtr vertexBuffer)
{
    m_VertexBuffer = vertexBuffer;
}

IIndexBufferPtr CMeshOGLES1::IndexBuffer() const
{
    return m_IndexBuffer;
}

void CMeshOGLES1::IndexBuffer(IIndexBufferPtr indexBuffer)
{
    m_IndexBuffer = indexBuffer;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGLES1