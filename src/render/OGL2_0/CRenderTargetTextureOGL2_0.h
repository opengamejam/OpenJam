//
//  CRenderTargetTextureOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0)

#ifndef CRENDERTARGETTEXTUREOGL2_0_H
#define CRENDERTARGETTEXTUREOGL2_0_H

#include "CRenderTargetTextureOGLBase.h"

namespace jam
{

class CRenderTargetTextureOGL2_0 : public CRenderTargetTextureOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetTextureOGL2_0();
    virtual ~CRenderTargetTextureOGL2_0();
};
    
}; // namespace jam

#endif /* CRENDERTARGETTEXTUREOGL2_0_H */

#endif /* defined(RENDER_OGL2_0) */
