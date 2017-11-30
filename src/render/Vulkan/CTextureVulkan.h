//
//  CTextureVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_VULKAN)

#ifndef CTEXTUREVULKAN_H
#define CTEXTUREVULKAN_H

#include "CTextureOGLBase.h"

namespace jam {

class CTextureVulkan : public CTextureOGLBase
{
    JAM_OBJECT
public:
    CTextureVulkan();
    virtual ~CTextureVulkan();

    /*
     * OpenGL specific
     */
    virtual GLfloat TextureFilterToGlFilter(ITexture::TextureFilters filter) override;
    virtual GLenum TexelFormatsToGlInternalFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelFormatsToGlFormat(TexelFormats texelFormat) override;
    virtual GLenum TexelTypeToGlType(TexelTypes texelType, TexelFormats texelFormat) override;
};

}; // namespace jam

#endif /* CTEXTUREVULKAN_H */

#endif /* defined(RENDER_VULKAN) */


