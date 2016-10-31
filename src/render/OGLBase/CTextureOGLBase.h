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
#define	CTEXTUREOGLBASE_H

#include "ITexture.h"
#include "IImage.h"

namespace jam
{

class CTextureOGLBase : public ITexture
{
public:
    CTextureOGLBase();
    virtual ~CTextureOGLBase();
    
    virtual bool AssignImage(IImagePtr image) override;
    
    virtual void Filter(TextureFilters filter) override;
    virtual TextureFilters Filter() const override;
    
    virtual void Bind() override;
    virtual void Unbind() override;
    virtual const std::string& Hash() override;
    
    /*
     * OpenGL specific
     */
    virtual GLfloat TextureFilterToGlFilter(ITexture::TextureFilters filter) = 0;
    virtual GLenum TexelFormatsToGlInternalFormat(TexelFormats texelFormat) = 0;
    virtual GLenum TexelFormatsToGlFormat(TexelFormats texelFormat) = 0;
    virtual GLenum TexelTypeToGlType(TexelTypes texelType, TexelFormats texelFormat) = 0;
    
protected:
    bool IsValid() const;
    
private:
    void HashMe();
    
private:
    uint32_t m_Id;

    TextureFilters m_Filter;
    std::string m_Hash;
    bool m_IsDirty;
};

}; // namespace jam

#endif /* CTEXTUREOGLBASE_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
