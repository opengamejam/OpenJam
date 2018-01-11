//
//  CMeshVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#include "CMeshVulkan.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMeshVulkan::CMeshVulkan()
{
}

CMeshVulkan::~CMeshVulkan()
{
}

void CMeshVulkan::Bind()
{
}

void CMeshVulkan::Unbind()
{
}

bool CMeshVulkan::IsValid() const
{
    return true;
}

IVertexBufferPtr CMeshVulkan::VertexBuffer() const
{
    return m_VertexBuffer;
}

void CMeshVulkan::VertexBuffer(IVertexBufferPtr vertexBuffer)
{
    m_VertexBuffer = vertexBuffer;
}

IIndexBufferPtr CMeshVulkan::IndexBuffer() const
{
    return m_IndexBuffer;
}

void CMeshVulkan::IndexBuffer(IIndexBufferPtr indexBuffer)
{
    m_IndexBuffer = indexBuffer;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

//#endif // RENDER_VULKAN
