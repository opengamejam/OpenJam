//
//  CMeshOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#ifndef CMESHOGLES1_1_H
#define CMESHOGLES1_1_H

#include "CMeshOGLBase.h"

namespace jam {

class CMeshOGLES1_1 : public CMeshOGLBase {
    JAM_OBJECT
public:
    CMeshOGLES1_1();
    virtual ~CMeshOGLES1_1();
};

}; // namespace jam

#endif /* CMESHOGLES1_1_H */

#endif /* defined(RENDER_OGLES1_1) */
