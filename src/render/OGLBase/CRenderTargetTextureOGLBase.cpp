//
//  CRenderTargetTextureOGLBase.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CRenderTargetTextureOGLBase.h"
#include "CTextureOGLBase.h"
#include "CImageRaw.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetTextureOGLBase::CRenderTargetTextureOGLBase()
{
}

CRenderTargetTextureOGLBase::~CRenderTargetTextureOGLBase()
{
}

void CRenderTargetTextureOGLBase::Initialize(InternalFormats internalFormat)
{
    if (IsInitialized()) {
        return;
    }
    m_Texture = CreateTextureObject();
}

void CRenderTargetTextureOGLBase::Shutdown()
{
    if (!IsInitialized()) {
        return;
    }
    m_Texture = nullptr;
}

bool CRenderTargetTextureOGLBase::IsInitialized()
{
    return (m_Texture != nullptr);
}

void CRenderTargetTextureOGLBase::Allocate(uint64_t width, uint64_t height)
{
    Texture()->Allocate(width, height);
    /*IResource::TResourceData textureData(width * height * 4);  // RGBA8888
     memset(&textureData[0], 0, textureData.size());
     IImagePtr image(new CImageRaw("sdfd", textureData, width, height, TexelProps(TF_RGBA8888, TT_UByte, 1)));
     Texture()->AssignImage(image);*/
}

void CRenderTargetTextureOGLBase::Bind() const
{
    assert(Texture());
    Texture()->Bind();
}

void CRenderTargetTextureOGLBase::Unbind() const
{
    assert(Texture());
    Texture()->Unbind();
}

ITexturePtr CRenderTargetTextureOGLBase::Texture() const
{
    return m_Texture;
}

void CRenderTargetTextureOGLBase::BindAsColorToFrameBuffer(uint64_t colorAttachementIdx)
{
    GLuint id = std::static_pointer_cast<CTextureOGLBase>(Texture())->TextureId();
    glFramebufferTexture2D(GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
        GL_TEXTURE_2D,
        id,
        0);
}

void CRenderTargetTextureOGLBase::BindAsDepthToFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        std::static_pointer_cast<CTextureOGLBase>(Texture())->TextureId(),
        0);
}

void CRenderTargetTextureOGLBase::BindAsStencilToFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
        GL_STENCIL_ATTACHMENT,
        GL_TEXTURE_2D,
        std::static_pointer_cast<CTextureOGLBase>(Texture())->TextureId(),
        0);
}

void CRenderTargetTextureOGLBase::UnbindAsColorFromFrameBuffer(uint64_t colorAttachementIdx)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
        GL_TEXTURE_2D,
        0,
        0);
}

void CRenderTargetTextureOGLBase::UnbindAsDepthFromFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        0,
        0);
}

void CRenderTargetTextureOGLBase::UnbindAsStencilFromFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
        GL_STENCIL_ATTACHMENT,
        GL_TEXTURE_2D,
        0,
        0);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
