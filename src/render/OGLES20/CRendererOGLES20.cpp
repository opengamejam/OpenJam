//
//  CRendererOGLES20.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#include "CRendererOGLES20.h"

#include "CVertexBufferOGLES20.h"
#include "CIndexBufferOGLES20.h"
#include "CMaterialOGLES20.h"
#include "CTextureOGLES20.h"
#include "CMeshOGLES20.h"
#include "CShaderOGLES20.h"
#include "CShaderProgramOGLES20.h"
#include "CFrameBufferTargetOGLES2.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type);

CRendererOGLES20::CRendererOGLES20(IRenderViewPtr renderView)
: m_RenderView(renderView)
{

}

CRendererOGLES20::~CRendererOGLES20()
{

}

IRenderViewPtr CRendererOGLES20::RenderView() const
{
    return m_RenderView;
}

IVertexBufferPtr CRendererOGLES20::CreatVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferOGLES20());
    return vertexBuffer;
}

IIndexBufferPtr CRendererOGLES20::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferOGLES20());
    return indexBuffer;
}

ITexturePtr CRendererOGLES20::CreateTexture()
{
    ITexturePtr texture(new CTextureOGLES20());
    return texture;
}

IMaterialPtr CRendererOGLES20::CreateMaterial()
{
    IMaterialPtr material(new CMaterialOGLES20());
    return material;
}

IMeshPtr CRendererOGLES20::CreateMesh()
{
    IMeshPtr mesh(new CMeshOGLES20());
    return mesh;
}

IShaderPtr CRendererOGLES20::CreateShader()
{
    IShaderPtr shader(new CShaderOGLES20());
    return shader;
}

IShaderProgramPtr CRendererOGLES20::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram(new CShaderProgramOGLES20());
    return shaderProgram;
}

IRenderTargetPtr CRendererOGLES20::CreateRenderTarget(unsigned int width, unsigned int height)
{
    IRenderTargetPtr renderTarget(new CFrameBufferTargetOGLES2(width, height));
    return renderTarget;
}

void CRendererOGLES20::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
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

void CRendererOGLES20::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGLES20::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !indexBuffer ||
        !material)
    {
        return;
    }
        
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawElements(primitiveType, (GLsizei)indexBuffer->Size(), GL_UNSIGNED_SHORT, (void *)0);
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

#endif // RENDER_OGLES2