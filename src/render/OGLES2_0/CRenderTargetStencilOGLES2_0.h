//
//  CRenderTargetStencilOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES2_0)

#ifndef CRENDERTARGETSTENCILOGLES2_0_H
#define CRENDERTARGETSTENCILOGLES2_0_H

#include "CRenderTargetStencilOGLBase.h"

namespace jam
{
    
class CRenderTargetStencilOGLES2_0 : public CRenderTargetStencilOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetStencilOGLES2_0();
    virtual ~CRenderTargetStencilOGLES2_0();
    
    /*
     * OpenGL specific
     */
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) override;
};
    
}; // namespace jam

#endif /* CRENDERTARGETSTENCILOGLES2_0_H */

#endif /* defined(RENDER_OGLES2_0) */
