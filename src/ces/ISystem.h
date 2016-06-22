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

/*
 * Interface ISystem
 */
class ISystem
{
    JAM_OBJECT_BASE
public:
    typedef std::set<IEntityPtr> TEntities;
    typedef std::set<typeid_t> TComponentIds;
    
public:
    ISystem() = default;
    virtual ~ISystem() = default;
    
    virtual void AddEntity(IEntityPtr entity) = 0;
    virtual void RemoveEntity(IEntityPtr entity) = 0;
    virtual const TEntities& Entities() const = 0;
    virtual bool IsEntityAdded(IEntityPtr entity) = 0;
    
    virtual const TComponentIds& RegisteredComponents() = 0;
    virtual bool IsComponentRegistered(typeid_t id) = 0;
    virtual bool HaveSupportedComponents(IEntityPtr entity) = 0;
    
    virtual void Update(unsigned long dt) = 0;
};
  
/*
 * Base class CSystemBase
 */
class CSystemBase : public ISystem
{
    JAM_OBJECT
public:
    CSystemBase();
    virtual ~CSystemBase();
    
    void AddEntity(IEntityPtr entity) override;
    void RemoveEntity(IEntityPtr entity) override;
    const TEntities& Entities() const override;
    bool IsEntityAdded(IEntityPtr entity) override;
    
    const TComponentIds& RegisteredComponents() override;
    bool IsComponentRegistered(typeid_t id) override;
    bool HaveSupportedComponents(IEntityPtr entity) override;
    
    virtual void Update(unsigned long dt) override = 0;
    
protected:
    void MarkDirtyEntity(IEntityPtr entity);
    const TEntities& DirtyEntities() const;
    void ClearDirtyEntities();
    
    template <typename T>
    void RegisterComponent()
    {
        T::OnAddedSignal += std::bind(&CSystemBase::OnAddedEntity, this, std::placeholders::_1);
        T::OnRemovedSignal += std::bind(&CSystemBase::OnRemovedEntity, this, std::placeholders::_1);
        T::OnChangedSignal += std::bind(&CSystemBase::OnChangedComponent, this, std::placeholders::_1);
        
        m_RegisteredComponents.insert(CTypeId<T>::Id());
    }
    
    template <typename T>
    void UnregisterComponent()
    {
        T::OnAddedSignal -= std::bind(&CSystemBase::OnAddedEntity, this, std::placeholders::_1);
        T::OnRemovedSignal -= std::bind(&CSystemBase::OnRemovedEntity, this, std::placeholders::_1);
        T::OnChangedSignal -= std::bind(&CSystemBase::OnChangedComponent, this, std::placeholders::_1);
        
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
