//
//  CRenderTargetDepthOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3)

#ifndef CRENDERTARGETDEPTHOGL1_3_H
#define CRENDERTARGETDEPTHOGL1_3_H

#include "CRenderTargetDepthOGLBase.h"

namespace jam {

class CRenderTargetDepthOGL1_3 : public CRenderTargetDepthOGLBase {
    JAM_OBJECT
public:
    CRenderTargetDepthOGL1_3();
    virtual ~CRenderTargetDepthOGL1_3();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) override;
    virtual CRenderTargetStencilPtr CreateStencilObject() override;
};

}; // namespace jam

#endif /* CRENDERTARGETDEPTHOGL1_3_H */

#endif /* defined(RENDER_OGL1_3) */
