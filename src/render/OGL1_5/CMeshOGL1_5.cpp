//
//  CMeshOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#include "CMeshOGL1_5.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMeshOGL1_5::CMeshOGL1_5()
: m_VertexBuffer(nullptr)
, m_IndexBuffer(nullptr)
{
    
}

CMeshOGL1_5::~CMeshOGL1_5()
{
    
}

void CMeshOGL1_5::Bind()
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

void CMeshOGL1_5::Unbind()
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

bool CMeshOGL1_5::IsValid() const
{
    return true;
}

IVertexBufferPtr CMeshOGL1_5::VertexBuffer() const
{
    return m_VertexBuffer;
}

void CMeshOGL1_5::VertexBuffer(IVertexBufferPtr vertexBuffer)
{
    m_VertexBuffer = vertexBuffer;
}

IIndexBufferPtr CMeshOGL1_5::IndexBuffer() const
{
    return m_IndexBuffer;
}

void CMeshOGL1_5::IndexBuffer(IIndexBufferPtr indexBuffer)
{
    m_IndexBuffer = indexBuffer;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL1_5 || RENDER_OGLES1_1