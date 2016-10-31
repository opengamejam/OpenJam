//
//  CRenderTargetColorOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0)

#ifndef CRENDERTARGETCOLOROGL2_0_H
#define CRENDERTARGETCOLOROGL2_0_H

#include "CRenderTargetColorOGLBase.h"

namespace jam
{

class CRenderTargetColorOGL2_0 : public CRenderTargetColorOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetColorOGL2_0();
    virtual ~CRenderTargetColorOGL2_0();
    
    /*
     * OpenGL specific
     */
    virtual GLenum ConvertInternalFormat(InternalFormats internalFormat) override;
};
    
}; // namespace jam

#endif /* CRENDERTARGETCOLOROGL2_0_H */

#endif /* defined(RENDER_OGL2_0) */
