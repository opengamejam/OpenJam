//
//  CRendererOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#include "CRendererOGL1_3.h"

#include "CVertexBufferOGL1_3.h"
#include "CIndexBufferOGL1_3.h"
#include "CMaterialOGL1_3.h"
#include "CTextureOGL1_3.h"
#include "CMeshOGL1_3.h"
#include "CShaderOGL1_3.h"
#include "CShaderProgramOGL1_3.h"
#include "CFrameBufferTargetOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type);

CRendererOGL1_3::CRendererOGL1_3(IRenderViewPtr renderView)
: m_RenderView(renderView)
{
    
}

CRendererOGL1_3::~CRendererOGL1_3()
{
    
}

IRenderViewPtr CRendererOGL1_3::RenderView() const
{
    return m_RenderView;
}

IVertexBufferPtr CRendererOGL1_3::CreatVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferOGL1_3());
    return vertexBuffer;
}

IIndexBufferPtr CRendererOGL1_3::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferOGL1_3());
    return indexBuffer;
}

ITexturePtr CRendererOGL1_3::CreateTexture()
{
    ITexturePtr texture(new CTextureOGL1_3());
    return texture;
}

IMaterialPtr CRendererOGL1_3::CreateMaterial()
{
    IMaterialPtr material(new CMaterialOGL1_3());
    return material;
}

IMeshPtr CRendererOGL1_3::CreateMesh()
{
    IMeshPtr mesh(new CMeshOGL1_3());
    return mesh;
}

IShaderPtr CRendererOGL1_3::CreateShader()
{
    IShaderPtr shader(new CShaderOGL1_3());
    return shader;
}

IShaderProgramPtr CRendererOGL1_3::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram(new CShaderProgramOGL1_3());
    return shaderProgram;
}

IRenderTargetPtr CRendererOGL1_3::CreateRenderTarget(unsigned int width, unsigned int height)
{
    IRenderTargetPtr renderTarget(new CFrameBufferTargetOGL1_3(width, height));
    return renderTarget;
}

void CRendererOGL1_3::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
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
        glLoadTransposeMatrixf(matrix().data());
    }   // TODO: load identity for other case
    
    it = uniforms.find(shader->ModelMatrix());
    if (it != uniforms.end())
    {
        CMatrix4x4f matrix = it->second;
        glMatrixMode(GL_MODELVIEW);
        glLoadTransposeMatrixf(matrix().data());
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

void CRendererOGL1_3::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGL1_3::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !indexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    const GLvoid *data = indexBuffer->LockRaw();
    glDrawElements(primitiveType, (GLsizei)indexBuffer->Size(), GL_UNSIGNED_SHORT, data);
    indexBuffer->Unlock();
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

#endif // RENDER_OGL1_3
