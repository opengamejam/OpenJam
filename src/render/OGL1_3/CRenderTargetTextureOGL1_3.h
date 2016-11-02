//
//  CRenderTargetTextureOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3)

#ifndef CRENDERTARGETTEXTUREOGL1_3_H
#define CRENDERTARGETTEXTUREOGL1_3_H

#include "CRenderTargetTextureOGLBase.h"

namespace jam {

class CRenderTargetTextureOGL1_3 : public CRenderTargetTextureOGLBase {
    JAM_OBJECT
public:
    CRenderTargetTextureOGL1_3();
    virtual ~CRenderTargetTextureOGL1_3();
    
    /*
     * OpenGL specific
     */
    virtual ITexturePtr CreateTextureObject() const override;
};

}; // namespace jam

#endif /* CRENDERTARGETTEXTUREOGL1_3_H */

#endif /* defined(RENDER_OGL1_3) */
