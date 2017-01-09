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

namespace jam {

CLASS_PTR(IEntity)
CLASS_PTR(IComponent)

/*
 * Interface IEntity
 */
class IEntity : public std::enable_shared_from_this<IEntity> {
    JAM_OBJECT_BASE
public:
    typedef std::vector<IComponentPtr> TComponentsList;
    typedef std::unordered_map<typeid_t, TComponentsList> TComponentsMap;
    typedef std::vector<IEntityPtr> TEntities;

public:
    IEntity() = default;
    virtual ~IEntity() = default;

    virtual void Initialize(const std::string& name, const TComponentsList& components) = 0;

    virtual const std::string& Name() const = 0;
    virtual void Name(const std::string& name) = 0;

    virtual void AddComponent(IComponentPtr component) = 0;
    virtual IComponentPtr GetComponent(typeid_t id) = 0;

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
        if (component && func) {
            func(component);
        }
    }

    virtual void RemoveComponent(IComponentPtr component) = 0;
    virtual void RemoveAllComponents(typeid_t id) = 0;
    virtual bool HasComponent(typeid_t id) = 0;
    virtual uint32_t ComponentsNum(typeid_t id) = 0;

    virtual void AddChild(IEntityPtr entity) = 0;
    virtual void RemoveChild(IEntityPtr entity) = 0;
    virtual const TEntities& Children() const = 0;

    virtual IEntityPtr Parent() const = 0;

    virtual uint32_t HierarchyIndex() const = 0;
};

/*
 * Base class CEntityBase
 */
class CEntityBase : public IEntity {
    JAM_OBJECT
public:
    CEntityBase();
    virtual ~CEntityBase();

    void Initialize(const std::string& name, const TComponentsList& components) override;

    const std::string& Name() const override;
    void Name(const std::string& name) override;

    void AddComponent(IComponentPtr component) override;
    IComponentPtr GetComponent(typeid_t id) override;

    void RemoveComponent(IComponentPtr component) override;
    void RemoveAllComponents(typeid_t id) override;
    bool HasComponent(typeid_t id) override;
    uint32_t ComponentsNum(typeid_t id) override;

    void AddChild(IEntityPtr entity) override;
    void RemoveChild(IEntityPtr entity) override;
    const TEntities& Children() const override;

    IEntityPtr Parent() const override;

    uint32_t HierarchyIndex() const override;

protected:
    void Parent(IEntityPtr parent);
    void HierarchyIndex(uint32_t hierarchyIndex);

private:
    std::string m_Name;
    TComponentsMap m_Components;
    TEntities m_Children;
    IEntityWeak m_Parent;
    uint32_t m_HierarchyIndex;
};

}; // namespace jam

#endif /* IENTITY_H */

