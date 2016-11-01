//
//  CRenderTargetColorOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_0)

#ifndef CRENDERTARGETCOLOROGLES1_0_H
#define CRENDERTARGETCOLOROGLES1_0_H

#include "CRenderTargetColorOGLBase.h"

namespace jam {

class CRenderTargetColorOGLES1_0 : public CRenderTargetColorOGLBase {
    JAM_OBJECT
public:
    CRenderTargetColorOGLES1_0();
    virtual ~CRenderTargetColorOGLES1_0();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertInternalFormat(InternalFormats internalFormat) override;
};

}; // namespace jam

#endif /* CRENDERTARGETCOLOROGLES1_0_H */

#endif /* defined(RENDER_OGLES1_0) */
