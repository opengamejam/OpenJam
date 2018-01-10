//
//  CTextureOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#ifndef CTEXTUREOGL2_0_H
#define CTEXTUREOGL2_0_H

#include "CTextureOGLBase.h"

namespace jam {

class CTextureOGL2_0 : public CTextureOGLBase {
    JAM_OBJECT
public:
    CTextureOGL2_0();
    virtual ~CTextureOGL2_0();

    /*
     * OpenGL specific
     */
    virtual GLfloat TextureFilterToGlFilter(ITexture::TextureFilters filter) override;
    virtual GLenum TexelFormatsToGlInternalFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelFormatsToGlFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelTypeToGlType(TexelTypes texelType, TexelFormats texelFormat) override;
};

}; // namespace jam

#endif /* CTEXTUREOGL2_0_H */

#endif /* defined(RENDER_OGL2_0) */


