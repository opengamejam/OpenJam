//
//  IEntity.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef IENTITY_H
#define IENTITY_H

#include "IComponent.h"

namespace jam
{

CLASS_PTR(IEntity);

class IEntity : public IEventable, public std::enable_shared_from_this<IEntity>
{
public:
    typedef std::vector<IComponentPtr> TComponentsList;
    typedef std::map<std::type_index, TComponentsList> TComponentsMap;
    typedef std::vector<IEntityPtr> TEntitiesList;
    
public:
    IEntity();
    virtual ~IEntity();
    
    template<class T>
    static std::shared_ptr<T> Create(const std::string& name, const TComponentsList& components)
    {
        std::shared_ptr<T> entity(new T());
        entity->Initialize(name, components);
        
        return entity;
    }
    
    void Initialize(const std::string& name, const TComponentsList& components);
    
    const std::string& Name() const;
    void Name(const std::string& name);
    
    void AddComponent(IComponentPtr component);
    IComponentPtr GetComponent(const std::type_index& id);

    template <class T>
    std::shared_ptr<T> Get()
    {
        std::type_index id = ComponentId<T>();
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
    void RemoveAllComponents(const std::type_index& id);
    bool HasComponent(const std::type_index& id);
    size_t ComponentsNum(const std::type_index& id);
    
    void AddChild(IEntityPtr entity);
    void RemoveChild(IEntityPtr entity);
    const TEntitiesList& Childs() const;

    void Parent(IEntityWeak parent);
    IEntityWeak Parent() const;
    
private:
    std::string m_Name;
    TComponentsMap m_Components;
    TEntitiesList m_Entities;
    IEntityWeak m_Parent;
};
    
}; // namespace jam

#endif /* IENTITY_H */