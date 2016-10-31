//
//  CRenderTargetStencilOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_5)

#ifndef CRENDERTARGETSTENCILOGL1_5_H
#define CRENDERTARGETSTENCILOGL1_5_H

#include "CRenderTargetStencilOGLBase.h"

namespace jam
{

class CRenderTargetStencilOGL1_5 : public CRenderTargetStencilOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetStencilOGL1_5();
    virtual ~CRenderTargetStencilOGL1_5();
    
    /*
     * OpenGL specific
     */
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) override;
};
    
}; // namespace jam

#endif /* CRENDERTARGETSTENCILOGL1_5_H */

#endif /* defined(RENDER_OGL1_5) */
