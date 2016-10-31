//
//  CMeshOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#ifndef CMESHOGLES2_0_H
#define CMESHOGLES2_0_H

#include "CMeshOGLBase.h"

namespace jam
{

class CMeshOGLES2_0 : public CMeshOGLBase
{
public:
    CMeshOGLES2_0();
    virtual ~CMeshOGLES2_0();
    
    virtual void Bind() override;
    virtual void Unbind() override;
    virtual bool IsValid() const override;
    
private:
    uint32_t m_VAO;
};

}; // namespace jam

#endif /* CMESHOGLES2_0_H */

#endif /* defined(RENDER_OGLES2_0) */
