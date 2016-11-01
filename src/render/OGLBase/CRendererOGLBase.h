//
//  CRendererOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CRENDEREROGLBASE_H
#define CRENDEREROGLBASE_H

#include "IRenderer.h"
#include "IMaterial.h"

namespace jam {

class CRendererOGLBase : public IRenderer {
public:
    CRendererOGLBase(IRenderViewPtr renderView);
    virtual ~CRendererOGLBase();

    /*
     * Returns render view
     */
    virtual IRenderViewPtr RenderView() const override;

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertPrimitiveType(IMaterial::PrimitiveTypes type);

private:
    IRenderViewPtr m_RenderView;
};

}; // namespace jam

#endif /* CRENDEREROGLBASE_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
