//
//  CRenderTargetColorOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_1)

#ifndef CRENDERTARGETCOLOROGLES1_1_H
#define CRENDERTARGETCOLOROGLES1_1_H

#include "CRenderTargetColorOGLBase.h"

namespace jam
{
    
class CRenderTargetColorOGLES1_1 : public CRenderTargetColorOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetColorOGLES1_1();
    virtual ~CRenderTargetColorOGLES1_1();
    
    /*
     * OpenGL specific
     */
    virtual GLenum ConvertInternalFormat(InternalFormats internalFormat) override;
};
    
}; // namespace jam

#endif /* CRENDERTARGETCOLOROGLES1_1_H */

#endif /* defined(RENDER_OGLES1_1) */
