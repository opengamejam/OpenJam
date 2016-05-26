//
//  ISystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef ISYSTEM_H
#define ISYSTEM_H

#include "IEventable.h"

namespace jam
{
    
CLASS_PTR(IEntity);
CLASS_PTR(IEvent);

class ISystem : public IEventable
{
    JAM_OBJECT_BASE
public:
    typedef std::set<IEntityPtr> TEntities;
    typedef std::set<typeid_t> TComponentIds;
    
public:
    ISystem();
    virtual ~ISystem();
    
    void AddEntity(IEntityPtr entity);
    void RemoveEntity(IEntityPtr entity);
    const TEntities& Entities() const;
    bool IsEntityAdded(IEntityPtr entity);
    
    const TComponentIds& RegisteredComponents();
    bool IsComponentRegistered(typeid_t id);
    bool HaveSupportedComponents(IEntityPtr entity);
    
    virtual void Update(unsigned long dt) = 0;
    
protected:
    void RegisterComponent(typeid_t id);
    void UnregisterComponent(typeid_t id);
    
    void MarkDirtyEntity(IEntityPtr entity);
    void UnmarkDirtyEntity(IEntityPtr entity);
    const TEntities& DirtyEntities() const;
    void ClearDirtyEntities();
    
    virtual void OnAddedEntity(IEntityPtr entity);
    virtual void OnChangedEntity(IEntityPtr entity);
    virtual void OnRemovedEntity(IEntityPtr entity);
    
private:
    bool OnComponentChanged(IEventPtr event);
    
private:
    TEntities m_Entities;
    TEntities m_DirtyEntities;
    TComponentIds m_RegisteredComponents;
};
    
}; // namespace jam

#endif /* ISYSTEM_H */
