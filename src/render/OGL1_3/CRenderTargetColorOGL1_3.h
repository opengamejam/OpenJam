//
//  CRenderTargetColorOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3)

#ifndef CRENDERTARGETCOLOROGL1_3_H
#define CRENDERTARGETCOLOROGL1_3_H

#include "CRenderTargetColorOGLBase.h"

namespace jam
{
    
class CRenderTargetColorOGL1_3 : public CRenderTargetColorOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetColorOGL1_3();
    virtual ~CRenderTargetColorOGL1_3();
    
    /*
     * OpenGL specific
     */
    virtual GLenum ConvertInternalFormat(InternalFormats internalFormat) override;
};
    
}; // namespace jam

#endif /* CRENDERTARGETCOLOROGL1_3_H */

#endif /* defined(RENDER_OGL1_3) */
