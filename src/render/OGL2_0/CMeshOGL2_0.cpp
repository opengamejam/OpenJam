//
//  CMeshOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#include "CMeshOGL2_0.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMeshOGL2_0::CMeshOGL2_0()
: m_VertexBuffer(nullptr)
, m_IndexBuffer(nullptr)
{
    
}

CMeshOGL2_0::~CMeshOGL2_0()
{
    
}

void CMeshOGL2_0::Bind()
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

void CMeshOGL2_0::Unbind()
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

bool CMeshOGL2_0::IsValid() const
{
    return true;
}

IVertexBufferPtr CMeshOGL2_0::VertexBuffer() const
{
    return m_VertexBuffer;
}

void CMeshOGL2_0::VertexBuffer(IVertexBufferPtr vertexBuffer)
{
    m_VertexBuffer = vertexBuffer;
}

IIndexBufferPtr CMeshOGL2_0::IndexBuffer() const
{
    return m_IndexBuffer;
}

void CMeshOGL2_0::IndexBuffer(IIndexBufferPtr indexBuffer)
{
    m_IndexBuffer = indexBuffer;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0