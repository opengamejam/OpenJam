//
//  CRenderTargetDepthOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0)

#ifndef CRENDERTARGETDEPTHOGL2_0_H
#define CRENDERTARGETDEPTHOGL2_0_H

#include "CRenderTargetDepthOGLBase.h"

namespace jam {

class CRenderTargetDepthOGL2_0 : public CRenderTargetDepthOGLBase {
    JAM_OBJECT
public:
    CRenderTargetDepthOGL2_0();
    virtual ~CRenderTargetDepthOGL2_0();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) override;
    virtual CRenderTargetStencilPtr CreateStencilObject() override;
};

}; // namespace jam

#endif /* CRENDERTARGETDEPTHOGL2_0_H */

#endif /* defined(RENDER_OGL2_0) */
