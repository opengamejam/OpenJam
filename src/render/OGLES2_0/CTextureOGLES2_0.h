//
//  CTextureOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#ifndef CTEXTUREOGLES2_0_H
#define CTEXTUREOGLES2_0_H

#include "CTextureOGLBase.h"

namespace jam {

class CTextureOGLES2_0 : public CTextureOGLBase {
public:
    CTextureOGLES2_0();
    virtual ~CTextureOGLES2_0();

    /*
     * OpenGL specific
     */
    virtual GLfloat TextureFilterToGlFilter(ITexture::TextureFilters filter) override;
    virtual GLenum TexelFormatsToGlInternalFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelFormatsToGlFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelTypeToGlType(TexelTypes texelType, TexelFormats texelFormat) override;
};

}; // namespace jam

#endif /* CTEXTUREOGLES2_0_H */

#endif /* defined(RENDER_OGLES2_0) */


