//
//  CTextureOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CTEXTUREOGL1_3_H
#define	CTEXTUREOGL1_3_H

#include "CTextureOGLBase.h"

namespace jam
{
    
class CTextureOGL1_3 : public CTextureOGLBase
{
public:
    CTextureOGL1_3();
    virtual ~CTextureOGL1_3();
    
    /*
     * OpenGL specific
     */
    virtual GLfloat TextureFilterToGlFilter(ITexture::TextureFilters filter) override;
    virtual GLenum TexelFormatsToGlInternalFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelFormatsToGlFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelTypeToGlType(TexelTypes texelType, TexelFormats texelFormat) override;
};
    
}; // namespace jam

#endif /* CTEXTUREOGL1_3_H */

#endif /* defined(RENDER_OGL1_3) */
