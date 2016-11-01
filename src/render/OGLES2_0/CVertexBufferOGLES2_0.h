//
//  CVertexBufferOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#ifndef CVERTEXBUFFEROGLES2_0_H
#define CVERTEXBUFFEROGLES2_0_H

#include "CVertexBufferOGLBase.h"

namespace jam {

class CVertexBufferOGLES2_0 : public CVertexBufferOGLBase {
public:
    CVertexBufferOGLES2_0();
    virtual ~CVertexBufferOGLES2_0();

    virtual void Bind() override;
    virtual void Unbind() override;
};

}; // namespace jam

#endif /* defined(CVERTEXBUFFEROGLES2_0_H) */

#endif /* defined(RENDER_OGLES2_0) */
