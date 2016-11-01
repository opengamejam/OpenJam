 
    
//
//  CRenderTargetDepthOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_1)

#ifndef CRENDERTARGETDEPTHOGLES1_1_H
#define CRENDERTARGETDEPTHOGLES1_1_H

#include "CRenderTargetDepthOGLBase.h"

namespace jam {

class CRenderTargetDepthOGLES1_1 : public CRenderTargetDepthOGLBase {
    JAM_OBJECT
public:
    CRenderTargetDepthOGLES1_1();
    virtual ~CRenderTargetDepthOGLES1_1();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) override;
    virtual CRenderTargetStencilPtr CreateStencilObject() override;
};

}; // namespace jam

#endif /* CRENDERTARGETDEPTHOGLES1_1_H */

#endif /* defined(RENDER_OGLES1_1) */

