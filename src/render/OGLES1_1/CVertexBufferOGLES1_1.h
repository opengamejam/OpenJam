//
//  CVertexBufferOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#ifndef CVERTEXBUFFEROGLES1_1_H
#define CVERTEXBUFFEROGLES1_1_H

#include "CVertexBufferOGLBase.h"

namespace jam {

class CVertexBufferOGLES1_1 : public CVertexBufferOGLBase {
public:
    CVertexBufferOGLES1_1();
    virtual ~CVertexBufferOGLES1_1();

    virtual void Bind() override;
    virtual void Unbind() override;
};

}; // namespace jam

#endif /* defined(CVERTEXBUFFEROGLES1_1_H) */

#endif /* defined(RENDER_OGLES1_1) */
