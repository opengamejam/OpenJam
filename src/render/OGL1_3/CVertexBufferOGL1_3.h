//
//  CVertexBufferOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CVERTEXBUFFEROGL1_3_H
#define CVERTEXBUFFEROGL1_3_H

#include "CVertexBufferOGLBase.h"

namespace jam {

class CVertexBufferOGL1_3 : public CVertexBufferOGLBase {
    JAM_OBJECT
public:
    CVertexBufferOGL1_3();
    virtual ~CVertexBufferOGL1_3();

    virtual void Bind() override;
    virtual void Unbind() override;
};

}; // namespace jam

#endif /* defined(CVERTEXBUFFEROGL1_3_H) */

#endif /* defined(RENDER_OGL1_3) */
