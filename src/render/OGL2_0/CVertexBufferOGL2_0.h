//
//  CVertexBufferOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#ifndef CVERTEXBUFFEROGL2_0_H
#define CVERTEXBUFFEROGL2_0_H

#include "CVertexBufferOGLBase.h"

namespace jam {

class CVertexBufferOGL2_0 : public CVertexBufferOGLBase {
    JAM_OBJECT
public:
    CVertexBufferOGL2_0();
    virtual ~CVertexBufferOGL2_0();

    virtual void Bind() override;
    virtual void Unbind() override;
};

}; // namespace jam

#endif /* defined(CVERTEXBUFFEROGL2_0_H) */

#endif /* defined(RENDER_OGL2_0) */
