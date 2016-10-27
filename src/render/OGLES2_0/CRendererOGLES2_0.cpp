//
//  CRendererOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#include "CRendererOGLES2_0.h"

#include "CVertexBufferOGLES2_0.h"
#include "CIndexBufferOGLES2_0.h"
#include "CMaterialOGLES2_0.h"
#include "CTextureOGLES2_0.h"
#include "CMeshOGLES2_0.h"
#include "CShaderOGLES2_0.h"
#include "CShaderProgramOGLES2_0.h"
#include "CFrameBufferOGLES2_0.h"
#include "CRenderTargetColorOGLES2_0.h"
#include "CRenderTargetDepthOGLES2_0.h"
#include "CRenderTargetStencilOGLES2_0.h"
#include "CRenderTargetTextureOGLES2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type);

CRendererOGLES2_0::CRendererOGLES2_0(IRenderViewPtr renderView)
: m_RenderView(renderView)
{
    
}

CRendererOGLES2_0::~CRendererOGLES2_0()
{
    
}

IRenderViewPtr CRendererOGLES2_0::RenderView() const
{
    return m_RenderView;
}

IFrameBufferPtr CRendererOGLES2_0::CreateFrameBuffer(uint32_t width, uint32_t height)
{
    IFrameBufferPtr frameBuffer(new CFrameBufferOGLES2_0(width, height));
    return frameBuffer;
}

CRenderTargetColorPtr CRendererOGLES2_0::CreateColorRenderTarget()
{
    CRenderTargetColorPtr colorTarget(new CRenderTargetColorOGLES2_0());
    return colorTarget;
}

CRenderTargetDepthPtr CRendererOGLES2_0::CreateDepthRenderTarget()
{
    CRenderTargetDepthPtr depthTarget(new CRenderTargetDepthOGLES2_0());
    return depthTarget;
}

CRenderTargetStencilPtr CRendererOGLES2_0::CreateStencilRenderTarget()
{
    CRenderTargetStencilPtr stencilTarget(new CRenderTargetStencilOGLES2_0());
    return stencilTarget;
}

CRenderTargetTexturePtr CRendererOGLES2_0::CreateTextureRenderTarget()
{
    CRenderTargetTexturePtr textureTarget(new CRenderTargetTextureOGLES2_0());
    return textureTarget;
}

IVertexBufferPtr CRendererOGLES2_0::CreateVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferOGLES2_0());
    return vertexBuffer;
}

IIndexBufferPtr CRendererOGLES2_0::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferOGLES2_0());
    return indexBuffer;
}

ITexturePtr CRendererOGLES2_0::CreateTexture()
{
    ITexturePtr texture(new CTextureOGLES2_0());
    return texture;
}

IMaterialPtr CRendererOGLES2_0::CreateMaterial()
{
    IMaterialPtr material(new CMaterialOGLES2_0());
    return material;
}

IMeshPtr CRendererOGLES2_0::CreateMesh()
{
    IMeshPtr mesh(new CMeshOGLES2_0());
    return mesh;
}

IShaderPtr CRendererOGLES2_0::CreateShader()
{
    IShaderPtr shader(new CShaderOGLES2_0());
    return shader;
}

IShaderProgramPtr CRendererOGLES2_0::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram(new CShaderProgramOGLES2_0());
    return shaderProgram;
}

void CRendererOGLES2_0::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
{
    if (!mesh ||
        !material)
    {
        return;
    }
    
    if (mesh->IndexBuffer())
    {
        Draw(mesh->VertexBuffer(), mesh->IndexBuffer(), material);
    }
    else
    {
        Draw(mesh->VertexBuffer(), material);
    }
}

void CRendererOGLES2_0::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGLES2_0::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !indexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawElements(primitiveType, (GLsizei)indexBuffer->Size(), GL_UNSIGNED_SHORT, nullptr);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type)
{
    int primitiveType = GL_TRIANGLES;
    switch (type)
    {
        case IMaterial::Points:
            primitiveType = GL_POINTS;
            break;
            
        case IMaterial::Lines:
            primitiveType = GL_LINES;
            break;
            
        case IMaterial::LinesLoop:
            primitiveType = GL_LINE_LOOP;
            break;
            
        case IMaterial::LinesStrip:
            primitiveType = GL_LINE_STRIP;
            break;
            
        case IMaterial::Triangles:
            primitiveType = GL_TRIANGLES;
            break;
            
        case IMaterial::TrianglesFan:
            primitiveType = GL_TRIANGLE_FAN;
            break;
            
        case IMaterial::TrianglesStrip:
            primitiveType = GL_TRIANGLE_STRIP;
            break;
            
        default:
            break;
    }
    
    return primitiveType;
}


#endif // RENDER_OGLES2_0
