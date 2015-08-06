//
//  CRendererOGLES1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1)

#include "CRendererOGLES1.h"

#include "CVertexBufferOGLES1.h"
#include "CIndexBufferOGLES1.h"
#include "CMaterialOGLES1.h"
#include "CTextureOGLES1.h"
#include "CMeshOGLES1.h"
#include "CShaderOGLES1.h"
#include "CShaderProgramOGLES1.h"
#include "CFrameBufferTargetOGLES1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type);

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRendererOGLES1::CRendererOGLES1(IRenderViewPtr renderView)
: m_RenderView(renderView)
{

}

CRendererOGLES1::~CRendererOGLES1()
{

}

IRenderViewPtr CRendererOGLES1::RenderView() const
{
    return m_RenderView;
}

IVertexBufferPtr CRendererOGLES1::CreatVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferOGLES1());
    return vertexBuffer;
}

IIndexBufferPtr CRendererOGLES1::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferOGLES1());
    return indexBuffer;
}

ITexturePtr CRendererOGLES1::CreateTexture()
{
    ITexturePtr texture(new CTextureOGLES1());
    return texture;
}

IMaterialPtr CRendererOGLES1::CreateMaterial()
{
    IMaterialPtr material(new CMaterialOGLES1());
    return material;
}

IMeshPtr CRendererOGLES1::CreateMesh()
{
    IMeshPtr mesh(new CMeshOGLES1());
    return mesh;
}

IShaderPtr CRendererOGLES1::CreateShader()
{
    IShaderPtr shader(new CShaderOGLES1());
    return shader;
}

IShaderProgramPtr CRendererOGLES1::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram(new CShaderProgramOGLES1());
    return shaderProgram;
}

IRenderTargetPtr CRendererOGLES1::CreateRenderTarget(unsigned int width, unsigned int height)
{
    IRenderTargetPtr renderTarget(new CFrameBufferTargetOGLES1(width, height));
    return renderTarget;
}

void CRendererOGLES1::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
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

void CRendererOGLES1::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGLES1::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !indexBuffer ||
        !material)
    {
        return;
    }
        
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawElements(primitiveType, (GLsizei)indexBuffer->Size(), GL_UNSIGNED_INT, (void *)0);
}

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type)
{
    int primitiveType = GL_TRIANGLES;
    switch (type)
    {
        case IMaterial::PT_Points:
            primitiveType = GL_POINTS;
            break;
            
        case IMaterial::PT_Lines:
            primitiveType = GL_LINES;
            break;
            
        case IMaterial::PT_LinesLoop:
            primitiveType = GL_LINE_LOOP;
            break;
            
        case IMaterial::PT_LinesStrip:
            primitiveType = GL_LINE_STRIP;
            break;
            
        case IMaterial::PT_Triangles:
            primitiveType = GL_TRIANGLES;
            break;
            
        case IMaterial::PT_TrianglesFan:
            primitiveType = GL_TRIANGLE_FAN;
            break;
            
        case IMaterial::PT_TrianglesStrip:
            primitiveType = GL_TRIANGLE_STRIP;
            break;
            
        default:
            break;
    }
    
    return primitiveType;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGLES1