//
//  CRenderTargetDepthOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_5)

#ifndef CRENDERTARGETDEPTHOGL1_5_H
#define CRENDERTARGETDEPTHOGL1_5_H

#include "CRenderTargetDepthOGLBase.h"

namespace jam
{

class CRenderTargetDepthOGL1_5 : public CRenderTargetDepthOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetDepthOGL1_5();
    virtual ~CRenderTargetDepthOGL1_5();
    
    /*
     * OpenGL specific
     */
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) override;
    virtual CRenderTargetStencilPtr CreateStencilObject() override;
};
    
}; // namespace jam

#endif /* CRENDERTARGETDEPTHOGL1_5_H */

#endif /* defined(RENDER_OGL1_5) */
