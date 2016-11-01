 
    
//
//  CRenderTargetDepthOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_0)

#ifndef CRENDERTARGETDEPTHOGLES1_0_H
#define CRENDERTARGETDEPTHOGLES1_0_H

#include "CRenderTargetDepthOGLBase.h"

namespace jam {

class CRenderTargetDepthOGLES1_0 : public CRenderTargetDepthOGLBase {
    JAM_OBJECT
public:
    CRenderTargetDepthOGLES1_0();
    virtual ~CRenderTargetDepthOGLES1_0();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) override;
    virtual CRenderTargetStencilPtr CreateStencilObject() override;
};

}; // namespace jam

#endif /* CRENDERTARGETDEPTHOGLES1_0_H */

#endif /* defined(RENDER_OGLES1_0) */

