//
//  CRenderTargetStencilOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0)

#ifndef CRENDERTARGETSTENCILOGL2_0_H
#define CRENDERTARGETSTENCILOGL2_0_H

#include "CRenderTargetStencilOGLBase.h"

namespace jam {

class CRenderTargetStencilOGL2_0 : public CRenderTargetStencilOGLBase {
    JAM_OBJECT
public:
    CRenderTargetStencilOGL2_0();
    virtual ~CRenderTargetStencilOGL2_0();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) override;
};

}; // namespace jam

#endif /* CRENDERTARGETSTENCILOGL2_0_H */

#endif /* defined(RENDER_OGL2_0) */
