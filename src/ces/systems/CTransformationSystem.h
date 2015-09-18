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
#include "IEntity.h"
#include "CTransform.h"

namespace jam
{
CLASS_PTR(CTransfromationSystem);
CLASS_PTR(CTransformationComponent);
    
class CTransfromationSystem : public ISystem
{
public:
    CTransfromationSystem();
    virtual ~CTransfromationSystem();
    
    virtual void Update(unsigned long dt) override;
    
private:
    void UpdateTransformsRecursively(IEntityPtr entity, const CTransform3Df& parentTransform);
};
    
} // namespace jam

#endif /* CTRANSFORMATIONSYSTEM_H */
