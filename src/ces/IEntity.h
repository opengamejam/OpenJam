//
//  IEntity.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef IENTITY_H
#define IENTITY_H

#include "Global.h"

namespace jam
{

CLASS_PTR(IEntity)
CLASS_PTR(IComponent)

class IEntity : public std::enable_shared_from_this<IEntity>
{
    JAM_OBJECT_BASE
public:
    typedef std::vector<IComponentPtr> TComponentsList;
    typedef std::unordered_map<typeid_t, TComponentsList> TComponentsMap;
    typedef std::vector<IEntityPtr> TEntities;
    
public:
    IEntity();
    virtual ~IEntity();
    
    void Initialize(const std::string& name, const TComponentsList& components);
    
    const std::string& Name() const;
    void Name(const std::string& name);
    
    void AddComponent(IComponentPtr component);
    IComponentPtr GetComponent(typeid_t id);

    template <class T>
    std::shared_ptr<T> Get()
    {
        typeid_t id = CTypeId<T>::Id();
        return std::static_pointer_cast<T>(GetComponent(id));
    }
    
    template <class T>
    void Get(std::function<void(std::shared_ptr<T>)> func)
    {
        std::shared_ptr<T> component = Get<T>();
        if (component && func)
        {
            func(component);
        }
    }
    
    void RemoveComponent(IComponentPtr component);
    void RemoveAllComponents(typeid_t id);
    bool HasComponent(typeid_t id);
    uint32_t ComponentsNum(typeid_t id);
    
    void AddChild(IEntityPtr entity);
    void RemoveChild(IEntityPtr entity);
    const TEntities& Childs() const;

    IEntityWeak Parent() const;
    
    uint32_t HierarchyIndex() const;
    
protected:
    void Parent(IEntityWeak parent);
    void HierarchyIndex(uint32_t hierarchyIndex);
    
private:
    std::string m_Name;
    TComponentsMap m_Components;
    TEntities m_Entities;
    IEntityWeak m_Parent;
    uint32_t m_HierarchyIndex;
};
    
}; // namespace jam

#endif /* IENTITY_H */
