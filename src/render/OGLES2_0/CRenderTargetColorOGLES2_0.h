//
//  CRenderTargetColorOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES2_0)

#ifndef CRENDERTARGETCOLOROGLES2_0_H
#define CRENDERTARGETCOLOROGLES2_0_H

#include "CRenderTargetColorOGLBase.h"

namespace jam
{
    
class CRenderTargetColorOGLES2_0 : public CRenderTargetColorOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetColorOGLES2_0();
    virtual ~CRenderTargetColorOGLES2_0();
    
    /*
     * OpenGL specific
     */
    virtual GLenum ConvertInternalFormat(InternalFormats internalFormat) override;
};
    
}; // namespace jam

#endif /* CRENDERTARGETCOLOROGLES2_0_H */

#endif /* defined(RENDER_OGLES2_0) */
