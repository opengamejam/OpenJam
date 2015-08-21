//
//  CRendererOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#include "CRendererOGLES1_1.h"

#include "CVertexBufferOGLES1_1.h"
#include "CIndexBufferOGLES1_1.h"
#include "CMaterialOGLES1_1.h"
#include "CTextureOGLES1_1.h"
#include "CMeshOGLES1_1.h"
#include "CShaderOGLES1_1.h"
#include "CShaderProgramOGLES1_1.h"
#include "CFrameBufferTargetOGLES1_1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type);

CRendererOGLES1_1::CRendererOGLES1_1(IRenderViewPtr renderView)
: m_RenderView(renderView)
{
    
}

CRendererOGLES1_1::~CRendererOGLES1_1()
{
    
}

IRenderViewPtr CRendererOGLES1_1::RenderView() const
{
    return m_RenderView;
}

IVertexBufferPtr CRendererOGLES1_1::CreatVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferOGLES1_1());
    return vertexBuffer;
}

IIndexBufferPtr CRendererOGLES1_1::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferOGLES1_1());
    return indexBuffer;
}

ITexturePtr CRendererOGLES1_1::CreateTexture()
{
    ITexturePtr texture(new CTextureOGLES1_1());
    return texture;
}

IMaterialPtr CRendererOGLES1_1::CreateMaterial()
{
    IMaterialPtr material(new CMaterialOGLES1_1());
    return material;
}

IMeshPtr CRendererOGLES1_1::CreateMesh()
{
    IMeshPtr mesh(new CMeshOGLES1_1());
    return mesh;
}

IShaderPtr CRendererOGLES1_1::CreateShader()
{
    IShaderPtr shader(new CShaderOGLES1_1());
    return shader;
}

IShaderProgramPtr CRendererOGLES1_1::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram(new CShaderProgramOGLES1_1());
    return shaderProgram;
}

IRenderTargetPtr CRendererOGLES1_1::CreateRenderTarget(unsigned int width, unsigned int height)
{
    IRenderTargetPtr renderTarget(new CFrameBufferTargetOGLES1_1(width, height));
    return renderTarget;
}

void CRendererOGLES1_1::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
{
    if (!mesh ||
        !material)
    {
        return;
    }
    
    const IMaterial::TUniMatrix4Float& uniforms = material->UniformsMatrix4x4f();
    IMaterial::TUniMatrix4Float::const_iterator it = uniforms.find(shader->ProjectionMatrix());
    if (it != uniforms.end())
    {
        CMatrix4x4f matrix = it->second;
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(matrix.Transpose()().data());
    }   // TODO: load identity for other case
    
    it = uniforms.find(shader->ModelMatrix());
    if (it != uniforms.end())
    {
        CMatrix4x4f matrix = it->second;
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(matrix.Transpose()().data());
    }   // TODO: load identity for other case
    
    if (mesh->IndexBuffer())
    {
        Draw(mesh->VertexBuffer(), mesh->IndexBuffer(), material);
    }
    else
    {
        Draw(mesh->VertexBuffer(), material);
    }
}

void CRendererOGLES1_1::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGLES1_1::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
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

#endif // RENDER_OGLES1_1