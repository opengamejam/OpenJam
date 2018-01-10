//
//  CMeshOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#ifndef CMESHOGL2_0_H
#define CMESHOGL2_0_H

#include "CMeshOGLBase.h"

namespace jam
{

class CMeshOGL2_0 : public CMeshOGLBase
{
    JAM_OBJECT
public:
    CMeshOGL2_0();
    virtual ~CMeshOGL2_0();
};

}; // namespace jam

#endif /* CMESHOGL2_0_H */

#endif /* defined(RENDER_OGL2_0) */
