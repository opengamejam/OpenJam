//
//  CVertexBufferOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5)

#ifndef CVERTEXBUFFEROGL1_5_H
#define CVERTEXBUFFEROGL1_5_H

#include "CVertexBufferOGLBase.h"

namespace jam {

class CVertexBufferOGL1_5 : public CVertexBufferOGLBase {
public:
    CVertexBufferOGL1_5();
    virtual ~CVertexBufferOGL1_5();

    virtual void Bind() override;
    virtual void Unbind() override;
};

}; // namespace jam

#endif /* defined(CVERTEXBUFFEROGL1_5_H) */

#endif /* defined(RENDER_OGL1_5) */
