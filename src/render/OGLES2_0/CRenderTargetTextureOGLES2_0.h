//
//  CRenderTargetTextureOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES2_0)

#ifndef CRENDERTARGETTEXTUREOGLES2_0_H
#define CRENDERTARGETTEXTUREOGLES2_0_H

#include "CRenderTargetTextureOGLBase.h"

namespace jam {

class CRenderTargetTextureOGLES2_0 : public CRenderTargetTextureOGLBase {
    JAM_OBJECT
public:
    CRenderTargetTextureOGLES2_0();
    virtual ~CRenderTargetTextureOGLES2_0();
    
    /*
     * OpenGL specific
     */
    virtual ITexturePtr CreateTextureObject() const override;
};

}; // namespace jam

#endif /* CRENDERTARGETTEXTUREOGLES2_0_H */

#endif /* defined(RENDER_OGLES2_0) */
