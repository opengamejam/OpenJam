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
#include "CRenderComponent.h"

namespace jam
{
CLASS_PTR(CRenderSystem);
CLASS_PTR(IEntity);
CLASS_PTR(ICamera);
CLASS_PTR(IRenderTarget);
CLASS_PTR(CRenderComponent);

class CRenderSystem : public ISystem
{
public:
    CRenderSystem();
    virtual ~CRenderSystem();
    
    virtual void Update(unsigned long dt) override;
    void Draw(ICameraPtr camera);
    
protected:
    virtual void OnAddedEntity(IEntityPtr entity) override;
    virtual void OnChangedEntity(IEntityPtr entity) override;
    virtual void OnRemovedEntity(IEntityPtr entity) override;
    
private:
    
    std::set<IRenderTargetPtr> m_ProccededRenderTargets;
    
    std::list<CRenderComponentPtr> m_SortedComponents;
    std::map<CRenderComponentPtr, uint64_t> m_OrderKeys;
};
    
} // namespace jam


#endif /* CRENDERSYSTEM_H */
