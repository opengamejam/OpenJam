//
//  CTextureOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#ifndef CTEXTUREOGLES1_0_H
#define	CTEXTUREOGLES1_0_H

#include "CTextureOGLBase.h"

namespace jam
{

class CTextureOGLES1_0 : public CTextureOGLBase
{
public:
    CTextureOGLES1_0();
    virtual ~CTextureOGLES1_0();
    
    /*
     * OpenGL specific
     */
    virtual GLfloat TextureFilterToGlFilter(ITexture::TextureFilters filter) override;
    virtual GLenum TexelFormatsToGlInternalFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelFormatsToGlFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelTypeToGlType(TexelTypes texelType, TexelFormats texelFormat) override;
};
    
}; // namespace jam

#endif /* CTEXTUREOGLES1_0_H */

#endif /* defined(RENDER_OGLES1_0) */
