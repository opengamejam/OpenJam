//
//  CRendererOGLES2.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#include "CRendererOGLES2.h"

#include "CVertexBufferOGLES2.h"
#include "CIndexBufferOGLES2.h"
#include "CMaterialOGLES2.h"
#include "CTextureOGLES2.h"
#include "CMeshOGLES2.h"
#include "CShaderOGLES2.h"
#include "CShaderProgramOGLES2.h"
#include "CFrameBufferTargetOGLES2.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type);

CRendererOGLES2::CRendererOGLES2(IRenderViewPtr renderView)
: m_RenderView(renderView)
{

}

CRendererOGLES2::~CRendererOGLES2()
{

}

IRenderViewPtr CRendererOGLES2::RenderView() const
{
    return m_RenderView;
}

IVertexBufferPtr CRendererOGLES2::CreatVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferOGLES2());
    return vertexBuffer;
}

IIndexBufferPtr CRendererOGLES2::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferOGLES2());
    return indexBuffer;
}

ITexturePtr CRendererOGLES2::CreateTexture()
{
    ITexturePtr texture(new CTextureOGLES2());
    return texture;
}

IMaterialPtr CRendererOGLES2::CreateMaterial()
{
    IMaterialPtr material(new CMaterialOGLES2());
    return material;
}

IMeshPtr CRendererOGLES2::CreateMesh()
{
    IMeshPtr mesh(new CMeshOGLES2());
    return mesh;
}

IShaderPtr CRendererOGLES2::CreateShader()
{
    IShaderPtr shader(new CShaderOGLES2());
    return shader;
}

IShaderProgramPtr CRendererOGLES2::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram(new CShaderProgramOGLES2());
    return shaderProgram;
}

IRenderTargetPtr CRendererOGLES2::CreateRenderTarget(unsigned int width, unsigned int height)
{
    IRenderTargetPtr renderTarget(new CFrameBufferTargetOGLES2(width, height));
    return renderTarget;
}

void CRendererOGLES2::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
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

void CRendererOGLES2::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGLES2::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
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