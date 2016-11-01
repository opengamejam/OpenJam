//
//  CRenderTargetStencilOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_0)

#ifndef CRENDERTARGETSTENCILOGLES1_0_H
#define CRENDERTARGETSTENCILOGLES1_0_H

#include "CRenderTargetStencilOGLBase.h"

namespace jam
{
    
class CRenderTargetStencilOGLES1_0 : public CRenderTargetStencilOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetStencilOGLES1_0();
    virtual ~CRenderTargetStencilOGLES1_0();
    
    /*
     * OpenGL specific
     */
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) override;
};
    
}; // namespace jam

#endif /* CRENDERTARGETSTENCILOGLES1_0_H */

#endif /* defined(RENDER_OGLES1_0) */
