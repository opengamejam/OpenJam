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
    
template<class T>
inline static std::type_index SystemId()
{
    return std::type_index(typeid(T));
}

class ISystem : public IEventable
{
public:
    typedef std::set<IEntityPtr> TEntitiesList;
    typedef std::set<std::type_index> TComponentIdsList;
    
public:
    ISystem();
    virtual ~ISystem();
    
    void AddEntity(IEntityPtr entity);
    void RemoveEntity(IEntityPtr entity);
    const TEntitiesList& Entities() const;
    bool IsEntityAdded(IEntityPtr entity);
    
    virtual void Update(unsigned long dt) = 0;
    
    const TComponentIdsList& RegisteredComponents();
    bool IsComponentRegistered(const std::type_index& id);
    bool IsHasSupportedComponents(IEntityPtr entity);
    
protected:
    void RegisterComponent(const std::type_index& id);
    void UnregisterComponent(const std::type_index& id);
    
    void MarkDirtyEntity(IEntityPtr entity);
    void UnmarkDirtyEntity(IEntityPtr entity);
    const TEntitiesList& DirtyEntities() const;
    void ClearDirtyEntities();
    
private:
    bool OnComponentChanged(IEventPtr event);
    
private:
    TEntitiesList m_Entities;
    TEntitiesList m_DirtyEntities;
    TComponentIdsList m_RegisteredComponents;
};
    
}; // namespace jam

#endif /* ISYSTEM_H */
