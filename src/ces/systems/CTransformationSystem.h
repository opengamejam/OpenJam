//
//  CTransfromationSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CTRANSFORMATIONSYSTEM_H
#define CTRANSFORMATIONSYSTEM_H

#include "ISystem.h"
#include "CTransformationComponent.h"


namespace jam {
CLASS_PTR(IEntity)
CLASS_PTR(CTransfromationSystem)

class CTransfromationSystem : public CSystemBase<CTransformationComponent> {
    JAM_OBJECT
public:
    CTransfromationSystem();
    virtual ~CTransfromationSystem();

    virtual void Update(unsigned long dt) override;
    void UpdateTransformsRecursively(CTransformationComponentPtr transformationComponent);
};

} // namespace jam

#endif /* CTRANSFORMATIONSYSTEM_H */
