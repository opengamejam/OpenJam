//
//  CRenderSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CANIMATION2DSYSTEM_H
#define CANIMATION2DSYSTEM_H

#include "ISystem.h"

namespace jam {
CLASS_PTR(CAnimation2DSystem)

class CAnimation2DSystem : public CSystemBase {
    JAM_OBJECT
public:
    CAnimation2DSystem();
    virtual ~CAnimation2DSystem();

    void Update(unsigned long dt) override;
};

} // namespace jam

#endif /* CANIMATION2DSYSTEM_H */
