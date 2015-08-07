//
//  CRenderSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CRENDERSYSTEM_H
#define CRENDERSYSTEM_H

#include "ISystem.h"

namespace jam
{
CLASS_PTR(CRenderSystem);
CLASS_PTR(IEntity);
CLASS_PTR(ICamera);
CLASS_PTR(CRenderComponent);
CLASS_PTR(IRenderTarget);

class CRenderSystem : public ISystem
{
public:
    CRenderSystem();
    virtual ~CRenderSystem();
    
    void Update(unsigned long dt);
    void Draw(ICameraPtr camera);
    
private:
    std::set<IRenderTargetPtr> m_ProccededRenderTargets;
};
    
} // namespace jam


#endif /* CRENDERSYSTEM_H */
