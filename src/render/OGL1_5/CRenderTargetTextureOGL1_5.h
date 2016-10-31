//
//  CRenderTargetTextureOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_5)

#ifndef CRENDERTARGETTEXTUREOGL1_5_H
#define CRENDERTARGETTEXTUREOGL1_5_H

#include "CRenderTargetTextureOGLBase.h"

namespace jam
{

class CRenderTargetTextureOGL1_5 : public CRenderTargetTextureOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetTextureOGL1_5();
    virtual ~CRenderTargetTextureOGL1_5();
};
    
}; // namespace jam

#endif /* CRENDERTARGETTEXTUREOGL1_5_H */

#endif /* defined(RENDER_OGL1_5) */
