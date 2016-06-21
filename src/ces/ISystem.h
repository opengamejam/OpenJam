//
//  ISystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef ISYSTEM_H
#define ISYSTEM_H

#include "Global.h"

namespace jam
{
    
CLASS_PTR(IEntity)
CLASS_PTR(IComponent)

class ISystem
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
    void MarkDirtyEntity(IEntityPtr entity);
    const TEntities& DirtyEntities() const;
    void ClearDirtyEntities();
    
    template <typename T>
    void RegisterComponent()
    {
        T::OnAddedSignal += std::bind(&ISystem::OnAddedEntity, this, std::placeholders::_1);
        T::OnRemovedSignal += std::bind(&ISystem::OnRemovedEntity, this, std::placeholders::_1);
        T::OnChangedSignal += std::bind(&ISystem::OnChangedComponent, this, std::placeholders::_1);
        
        m_RegisteredComponents.insert(CTypeId<T>::Id());
    }
    
    template <typename T>
    void UnregisterComponent()
    {
        T::OnAddedSignal -= std::bind(&ISystem::OnAddedEntity, this, std::placeholders::_1);
        T::OnRemovedSignal -= std::bind(&ISystem::OnRemovedEntity, this, std::placeholders::_1);
        T::OnChangedSignal -= std::bind(&ISystem::OnChangedComponent, this, std::placeholders::_1);
        
        m_RegisteredComponents.erase(CTypeId<T>::Id());
    }
    
    virtual void OnAddedEntity(IEntityPtr entity);
    virtual void OnRemovedEntity(IEntityPtr entity);
    virtual void OnChangedComponent(IComponentPtr component);
    
private:
    TEntities m_Entities;
    TEntities m_DirtyEntities;
    TComponentIds m_RegisteredComponents;
};
    
}; // namespace jam

#endif /* ISYSTEM_H */
