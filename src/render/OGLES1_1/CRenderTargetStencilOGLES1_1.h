//
//  CRenderTargetStencilOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_1)

#ifndef CRENDERTARGETSTENCILOGLES1_1_H
#define CRENDERTARGETSTENCILOGLES1_1_H

#include "CRenderTargetStencilOGLBase.h"

namespace jam
{
    
class CRenderTargetStencilOGLES1_1 : public CRenderTargetStencilOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetStencilOGLES1_1();
    virtual ~CRenderTargetStencilOGLES1_1();
    
    /*
     * OpenGL specific
     */
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) override;
};
    
}; // namespace jam

#endif /* CRENDERTARGETSTENCILOGLES1_1_H */

#endif /* defined(RENDER_OGLES1_1) */
