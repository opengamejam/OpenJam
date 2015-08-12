//
//  CRendererOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#include "CRendererOGLES1_0.h"

#include "CVertexBufferOGLES1_0.h"
#include "CIndexBufferOGLES1_0.h"
#include "CMaterialOGLES1_0.h"
#include "CTextureOGLES1_0.h"
#include "CMeshOGLES1_0.h"
#include "CShaderOGLES1_0.h"
#include "CShaderProgramOGLES1_0.h"
#include "CFrameBufferTargetOGLES1_0.h"

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

IVertexBufferPtr CRendererOGLES1_0::CreatVertexBuffer()
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

IRenderTargetPtr CRendererOGLES1_0::CreateRenderTarget(unsigned int width, unsigned int height)
{
    IRenderTargetPtr renderTarget(new CFrameBufferTargetOGLES1_0(width, height));
    return renderTarget;
}

void CRendererOGLES1_0::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
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

void CRendererOGLES1_0::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glBegin(primitiveType);
    
    const IVertexBuffer::TVertexStreamMap& vertexStreams = VertexStreams();
    std::for_each(vertexStreams.begin(), vertexStreams.end(),
                  [&](const IVertexBuffer::TVertexStreamMap::value_type& value)
                  {
                      const SVertexStream& stream = value.second;
                      if (stream.IsActive())
                      {
                          IVertexBufferPtr vertexBuffer = stream.vertexBuffer.lock();
                          if (vertexBuffer)
                          {
                              size_t size = vertexBuffer->Size();
                              
                          }
                          
                          /*GLbyte *offset = nullptr;
                           offset += value.second.offset;
                           if (value.first == IVertexBuffer::Position)
                           {
                           glEnableClientState(GL_VERTEX_ARRAY);
                           glVertexPointer(value.second.stride,
                           GL_FLOAT,   // TODO: parse type
                           (GLsizei)ElementSize(),
                           (GLvoid*)offset);
                           }
                           else if (value.first == IVertexBuffer::TextureCoors)
                           {
                           glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                           glTexCoordPointer(value.second.stride,
                           GL_FLOAT,   // TODO: parse type
                           (GLsizei)ElementSize(),
                           (GLvoid*)offset);
                           }
                           else if (value.first == IVertexBuffer::Color)
                           {
                           glEnableClientState(GL_COLOR_ARRAY);
                           glColorPointer(value.second.stride,
                           GL_FLOAT,   // TODO: parse type
                           (GLsizei)ElementSize(),
                           (GLvoid*)offset);
                           }
                           else if (value.first == IVertexBuffer::Normal)
                           {
                           glEnableClientState(GL_NORMAL_ARRAY);
                           glNormalPointer(GL_FLOAT,   // TODO: parse type
                           (GLsizei)ElementSize(),
                           (GLvoid*)offset);
                           }
                           else if (value.first == IVertexBuffer::Tangent)
                           {
                           }*/
                      }
                  });
    
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
    
    glEnd();
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
    //glDrawElements(primitiveType, (GLsizei)indexBuffer->Size(), GL_UNSIGNED_INT, (void *)0);
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

#endif // RENDER_OGLES1_0