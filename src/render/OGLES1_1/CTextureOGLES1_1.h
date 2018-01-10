//
//  CTextureOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#ifndef CTEXTUREOGLES1_1_H
#define CTEXTUREOGLES1_1_H

#include "CTextureOGLBase.h"

namespace jam {

class CTextureOGLES1_1 : public CTextureOGLBase {
    JAM_OBJECT
public:
    CTextureOGLES1_1();
    virtual ~CTextureOGLES1_1();

    /*
     * OpenGL specific
     */
    virtual GLfloat TextureFilterToGlFilter(ITexture::TextureFilters filter) override;
    virtual GLenum TexelFormatsToGlInternalFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelFormatsToGlFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelTypeToGlType(TexelTypes texelType, TexelFormats texelFormat) override;
};

}; // namespace jam

#endif /* CTEXTUREOGLES1_1_H */

#endif /* defined(RENDER_OGLES1_1) */


