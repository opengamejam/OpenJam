//
//  CTextureOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CTEXTUREOGLBASE_H
#define CTEXTUREOGLBASE_H

#include "ITexture.h"
#include "IImage.h"

namespace jam {

class CTextureOGLBase : public ITexture {
    JAM_OBJECT
public:
    CTextureOGLBase();
    virtual ~CTextureOGLBase();

    /*
     * Allocate render buffer with 'width' and 'height'
     */
    virtual void Allocate(uint64_t width, uint64_t height) override;
    
    /*
     * Assign image to texture
     */
    virtual bool AssignImage(IImagePtr image) override;

    virtual void Filter(TextureFilters filter) override;
    virtual TextureFilters Filter() const override;

    virtual void Bind() override;
    virtual void Unbind() override;
    virtual const std::string& Hash() override;

    /*
     * OpenGL specific
     */
    virtual GLuint TextureId() const;
    virtual GLfloat TextureFilterToGlFilter(ITexture::TextureFilters filter) = 0;
    virtual GLenum TexelFormatsToGlInternalFormat(TexelFormats texelFormat) = 0;
    virtual GLenum TexelFormatsToGlFormat(TexelFormats texelFormat) = 0;
    virtual GLenum TexelTypeToGlType(TexelTypes texelType, TexelFormats texelFormat) = 0;

protected:
    bool IsValid() const;

private:
    void HashMe();

private:
    GLuint m_Id;

    TextureFilters m_Filter;
    std::string m_Hash;
    bool m_IsDirty;
};

}; // namespace jam

#endif /* CTEXTUREOGLBASE_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
