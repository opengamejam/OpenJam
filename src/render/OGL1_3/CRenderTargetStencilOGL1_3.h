//
//  CRenderTargetStencilOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3)

#ifndef CRENDERTARGETSTENCILOGL1_3_H
#define CRENDERTARGETSTENCILOGL1_3_H

#include "CRenderTargetStencilOGLBase.h"

namespace jam
{
    
    class CRenderTargetStencilOGL1_3 : public CRenderTargetStencilOGLBase
    {
        JAM_OBJECT
    public:
        CRenderTargetStencilOGL1_3();
        virtual ~CRenderTargetStencilOGL1_3();
        
        /*
         * OpenGL specific
         */
        virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) override;
    };
    
}; // namespace jam

#endif /* CRENDERTARGETSTENCILOGL1_3_H */

#endif /* defined(RENDER_OGL1_3) */
