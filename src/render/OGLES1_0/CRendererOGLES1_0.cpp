//
//  CRendererOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#include "CRendererOGLES1_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type);

CRendererOGLES1_0::CRendererOGLES1_0(IRenderViewPtr renderView)
: m_RenderView(renderView)
{
    
}

CRendererOGLES1_0::~CRendererOGLES1_0()
{
    
}

IRenderViewPtr CRendererOGLES1_0::RenderView() const
{
    return m_RenderView;
}

IFrameBufferPtr CRendererOGLES1_0::CreateFrameBuffer(uint32_t width, uint32_t height)
{
    IFrameBufferPtr frameBuffer(new CFrameBufferOGLES1_0(width, height));
    return frameBuffer;
}

CRenderTargetColorPtr CRendererOGLES1_0::CreateColorRenderTarget()
{
    CRenderTargetColorPtr colorTarget(new CRenderTargetColorOGLES1_0());
    return colorTarget;
}

CRenderTargetDepthPtr CRendererOGLES1_0::CreateDepthRenderTarget()
{
    CRenderTargetDepthPtr depthTarget(new CRenderTargetDepthOGLES1_0());
    return depthTarget;
}

CRenderTargetStencilPtr CRendererOGLES1_0::CreateStencilRenderTarget()
{
    CRenderTargetStencilPtr stencilTarget(new CRenderTargetStencilOGLES1_0());
    return stencilTarget;
}

CRenderTargetTexturePtr CRendererOGLES1_0::CreateTextureRenderTarget()
{
    CRenderTargetTexturePtr textureTarget(new CRenderTargetTextureOGLES1_0());
    return textureTarget;
}

IVertexBufferPtr CRendererOGLES1_0::CreateVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferOGLES1_0());
    return vertexBuffer;
}

IIndexBufferPtr CRendererOGLES1_0::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferOGLES1_0());
    return indexBuffer;
}

ITexturePtr CRendererOGLES1_0::CreateTexture()
{
    ITexturePtr texture(new CTextureOGLES1_0());
    return texture;
}

IMaterialPtr CRendererOGLES1_0::CreateMaterial()
{
    IMaterialPtr material(new CMaterialOGLES1_0());
    return material;
}

IMeshPtr CRendererOGLES1_0::CreateMesh()
{
    IMeshPtr mesh(new CMeshOGLES1_0());
    return mesh;
}

IShaderPtr CRendererOGLES1_0::CreateShader()
{
    IShaderPtr shader(new CShaderOGLES1_0());
    return shader;
}

IShaderProgramPtr CRendererOGLES1_0::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram(new CShaderProgramOGLES1_0());
    return shaderProgram;
}

void CRendererOGLES1_0::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
{
    if (!mesh ||
        !material)
    {
        return;
    }
    
    const IShaderProgram::TUniMatrix4Float& uniforms = shader->UniformsMatrix4x4f();
    IShaderProgram::TUniMatrix4Float::const_iterator it = uniforms.find(shader->ProjectionMatrix());
    if (it != uniforms.end())
    {
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(it->second));
    }
    else
    {
        glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));
    }
    
    it = uniforms.find(shader->ModelMatrix());
    if (it != uniforms.end())
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(it->second));
    }
    else
    {
        glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));
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

void CRendererOGLES1_0::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGLES1_0::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !indexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawElements(primitiveType, (GLsizei)indexBuffer->Size(), GL_UNSIGNED_INT, nullptr);
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

#endif // defined(RENDER_OGLES1_0)