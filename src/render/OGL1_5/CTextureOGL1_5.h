//
//  CTextureOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5)

#ifndef CTEXTUREOGL1_5_H
#define CTEXTUREOGL1_5_H

#include "CTextureOGLBase.h"

namespace jam {

class CTextureOGL1_5 : public CTextureOGLBase {
public:
    CTextureOGL1_5();
    virtual ~CTextureOGL1_5();

    /*
     * OpenGL specific
     */
    virtual GLfloat TextureFilterToGlFilter(ITexture::TextureFilters filter) override;
    virtual GLenum TexelFormatsToGlInternalFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelFormatsToGlFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelTypeToGlType(TexelTypes texelType, TexelFormats texelFormat) override;
};

}; // namespace jam

#endif /* CTEXTUREOGL1_5_H */

#endif /* defined(RENDER_OGL1_5) */


