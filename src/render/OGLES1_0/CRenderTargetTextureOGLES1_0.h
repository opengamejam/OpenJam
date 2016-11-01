//
//  CRenderTargetTextureOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_0)

#ifndef CRENDERTARGETTEXTUREOGLES1_0_H
#define CRENDERTARGETTEXTUREOGLES1_0_H

#include "CRenderTargetTextureOGLBase.h"

namespace jam {

class CRenderTargetTextureOGLES1_0 : public CRenderTargetTextureOGLBase {
    JAM_OBJECT
public:
    CRenderTargetTextureOGLES1_0();
    virtual ~CRenderTargetTextureOGLES1_0();
};

}; // namespace jam

#endif /* CRENDERTARGETTEXTUREOGLES1_0_H */

#endif /* defined(RENDER_OGLES1_0) */
