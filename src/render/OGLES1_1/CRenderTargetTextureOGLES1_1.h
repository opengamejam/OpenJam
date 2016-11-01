//
//  CRenderTargetTextureOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_1)

#ifndef CRENDERTARGETTEXTUREOGLES1_1_H
#define CRENDERTARGETTEXTUREOGLES1_1_H

#include "CRenderTargetTextureOGLBase.h"

namespace jam
{
    
class CRenderTargetTextureOGLES1_1 : public CRenderTargetTextureOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetTextureOGLES1_1();
    virtual ~CRenderTargetTextureOGLES1_1();
};
    
}; // namespace jam

#endif /* CRENDERTARGETTEXTUREOGLES1_1_H */

#endif /* defined(RENDER_OGLES1_1) */
