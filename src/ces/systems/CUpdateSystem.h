//
//  CUpdateSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CUPDATESYSTEM_H
#define CUPDATESYSTEM_H

#include "ISystem.h"
#include "CUpdateComponent.h"

namespace jam {
CLASS_PTR(CUpdateSystem)

class CUpdateSystem : public CSystemBase<CUpdateComponent> {
    JAM_OBJECT
public:
    CUpdateSystem();
    virtual ~CUpdateSystem();

    virtual void Update(unsigned long dt) override;
};

} // namespace jam

#endif /* CUPDATESYSTEM_H */
