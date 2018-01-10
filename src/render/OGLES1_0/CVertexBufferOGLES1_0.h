//
//  CVertexBufferOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#ifndef CVERTEXBUFFEROGLES1_0_H
#define CVERTEXBUFFEROGLES1_0_H

#include "CVertexBufferOGLBase.h"

namespace jam {

class CVertexBufferOGLES1_0 : public CVertexBufferOGLBase {
    JAM_OBJECT
public:
    CVertexBufferOGLES1_0();
    virtual ~CVertexBufferOGLES1_0();

    virtual void Bind() override;
    virtual void Unbind() override;
};

}; // namespace jam

#endif /* defined(CVERTEXBUFFEROGLES1_0_H) */

#endif /* defined(RENDER_OGLES1_0) */
