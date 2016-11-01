//
//  CMeshOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5)

#ifndef CMESHOGL1_5_H
#define CMESHOGL1_5_H

#include "CMeshOGLBase.h"

namespace jam {

class CMeshOGL1_5 : public CMeshOGLBase {
public:
    CMeshOGL1_5();
    virtual ~CMeshOGL1_5();
};

}; // namespace jam

#endif /* CMESHOGL1_5_H */

#endif /* defined(RENDER_OGL1_5) */
