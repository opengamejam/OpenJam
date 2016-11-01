//
//  CMeshOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#ifndef CMESHOGLES1_0_H
#define CMESHOGLES1_0_H

#include "CMeshOGLBase.h"

namespace jam
{

class CMeshOGLES1_0 : public CMeshOGLBase
{
public:
    CMeshOGLES1_0();
    virtual ~CMeshOGLES1_0();
};
    
}; // namespace jam

#endif /* CMESHOGLES1_0_H */

#endif /* defined(RENDER_OGLES1_0) */
