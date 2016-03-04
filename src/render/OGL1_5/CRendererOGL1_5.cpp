//
//  CRendererOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5)

#include "CRendererOGL1_5.h"

#include "CVertexBufferOGL1_5.h"
#include "CIndexBufferOGL1_5.h"
#include "CMaterialOGL1_5.h"
#include "CTextureOGL1_5.h"
#include "CMeshOGL1_5.h"
#include "CShaderOGL1_5.h"
#include "CShaderProgramOGL1_5.h"
#include "CFrameBufferOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type);

CRendererOGL1_5::CRendererOGL1_5(IRenderViewPtr renderView)
: m_RenderView(renderView)
{

}

CRendererOGL1_5::~CRendererOGL1_5()
{

}

IRenderViewPtr CRendererOGL1_5::RenderView() const
{
    return m_RenderView;
}

IVertexBufferPtr CRendererOGL1_5::CreatVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferOGL1_5());
    return vertexBuffer;
}

IIndexBufferPtr CRendererOGL1_5::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferOGL1_5());
    return indexBuffer;
}

ITexturePtr CRendererOGL1_5::CreateTexture()
{
    ITexturePtr texture(new CTextureOGL1_5());
    return texture;
}

IMaterialPtr CRendererOGL1_5::CreateMaterial()
{
    IMaterialPtr material(new CMaterialOGL1_5());
    return material;
}

IMeshPtr CRendererOGL1_5::CreateMesh()
{
    IMeshPtr mesh(new CMeshOGL1_5());
    return mesh;
}

IShaderPtr CRendererOGL1_5::CreateShader()
{
    IShaderPtr shader(new CShaderOGL1_5());
    return shader;
}

IShaderProgramPtr CRendererOGL1_5::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram(new CShaderProgramOGL1_5());
    return shaderProgram;
}

IRenderTargetPtr CRendererOGL1_5::CreateRenderTarget(unsigned int width, unsigned int height)
{
    IRenderTargetPtr renderTarget(new CFrameBufferOGL1_5(width, height));
    return renderTarget;
}

void CRendererOGL1_5::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
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
        glLoadMatrixf(glm::value_ptr(glm::mat4(1.0)));
    }
    
    it = uniforms.find(shader->ModelMatrix());
    if (it != uniforms.end())
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(it->second));
    }
    else
    {
        glLoadMatrixf(glm::value_ptr(glm::mat4(1.0)));
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

void CRendererOGL1_5::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGL1_5::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
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

#endif // RENDER_OGL1_5 || RENDER_OGLES1_1