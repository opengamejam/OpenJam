//
//  CRenderTargetColorOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_5)

#ifndef CRENDERTARGETCOLOROGL1_5_H
#define CRENDERTARGETCOLOROGL1_5_H

#include "CRenderTargetColorOGLBase.h"

namespace jam {

class CRenderTargetColorOGL1_5 : public CRenderTargetColorOGLBase {
    JAM_OBJECT
public:
    CRenderTargetColorOGL1_5();
    virtual ~CRenderTargetColorOGL1_5();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertInternalFormat(InternalFormats internalFormat) override;
};

}; // namespace jam

#endif /* CRENDERTARGETCOLOROGL1_5_H */

#endif /* defined(RENDER_OGL1_5) */
