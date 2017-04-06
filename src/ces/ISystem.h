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
#include "IComponent.h"

namespace jam {

CLASS_PTR(IEntity)

/*
 * Interface ISystem
 */
class ISystem {
    JAM_OBJECT_BASE
public:
    typedef std::set<IEntityPtr> TEntitiesList;

public:
    /*
     * Constructor
     */
    ISystem() = default;
    
    /*
     * Destructor
     */
    virtual ~ISystem() = default;

    /*
     * Set of all component objects
     */
    virtual const std::set<IComponentPtr>& Components() const = 0;

    /*
     * Set of all component objects
     */
    virtual typeid_t RequiredComponent() const = 0;
    
    /*
     * Check if system supports component with 'ctype'
     */
    virtual bool IsSupportComponent(typeid_t id) = 0;
    
    /*
     * Template version of IsSupportedComponents
     */
    template <typename T>
    bool IsSupportComponent()
    {
        return IsSupportComponent(CTypeId<T>::Id());
    }

    /*
     * Update method for system
     */
    virtual void Update(unsigned long dt) = 0;
};

/*
 * Base class CSystemBase
 */
template <typename ComponentType>
class CSystemBase : public ISystem {
    JAM_OBJECT
public:
    /*
     * Constructor. When system is creating to corresponding component registers signals to handle adding, removing
     * or changing components
     */
    CSystemBase()
        : m_RequiredComponent(CTypeId<ComponentType>::Id())
        , m_MainThreadId(std::this_thread::get_id())
    {
        ComponentType::OnAddedSignal += std::bind(&CSystemBase::OnAddedComponent, this, std::placeholders::_1);
        ComponentType::OnRemovedSignal += std::bind(&CSystemBase::OnRemovedComponent, this, std::placeholders::_1);
        ComponentType::OnChangedSignal += std::bind(&CSystemBase::OnChangedComponent, this, std::placeholders::_1);
    }
    
    /*
     * Destructor. All registered signals removes
     */
    virtual ~CSystemBase()
    {
        ComponentType::OnAddedSignal -= std::bind(&CSystemBase::OnAddedComponent, this, std::placeholders::_1);
        ComponentType::OnRemovedSignal -= std::bind(&CSystemBase::OnRemovedComponent, this, std::placeholders::_1);
        ComponentType::OnChangedSignal -= std::bind(&CSystemBase::OnChangedComponent, this, std::placeholders::_1);
    }

    /*
     * Set of all component objects
     */
    const std::set<IComponentPtr>& Components() const override
    {
        return m_Components;
    }

    /*
     * Returns typeid of required component
     */
    typeid_t RequiredComponent() const override
    {
        return m_RequiredComponent;
    }
    
    /*
     * Check if system supports component with 'ctype'
     */
    bool IsSupportComponent(typeid_t ctype) override
    {
        return ctype == m_RequiredComponent;
    }

    /*
     * Update method for system
     */
    virtual void Update(unsigned long dt) override = 0;

protected:
    /*
     * Set of components, that was canged
     */
    const std::set<IComponentPtr>& DirtyComponents() const
    {
        return m_DirtyComponents;
    }
    
    /*
     * Clear all changed components
     */
    void ClearDirtyComponents()
    {
        m_DirtyComponents.clear();
    }

    /*
     * If component added to entity it notifies system by calling this method
     */
    virtual void OnAddedComponent(IComponentPtr component)
    {
        assert("Adding component from background thread is not allowed" &&
               m_MainThreadId == std::this_thread::get_id());
        
        if (IsSupportComponent(component->GetId())) {
            m_Components.insert(component);
        }
    }
    
    /*
     * If component removed to entity it notifies system by calling this method
     */
    virtual void OnRemovedComponent(IComponentPtr component)
    {
        assert("Removing component from background thread is not allowed" &&
               m_MainThreadId == std::this_thread::get_id());
        
        if (IsSupportComponent(component->GetId())) {
            m_Components.erase(component);
        }
    }
    
    /*
     * If component changed it notifies system by calling this method
     */
    virtual void OnChangedComponent(IComponentPtr component)
    {
        assert("Changing component from background thread is not allowed" &&
               m_MainThreadId == std::this_thread::get_id());
        
        if (IsSupportComponent(component->GetId())) {
            m_DirtyComponents.insert(component);
        }
    }
    
    /*
     * Iterate over all components and stop by condition with casting component to a concrete type
     */
    void AllOfComponents(std::function<bool(std::shared_ptr<ComponentType>)> func)
    {
        const std::set<IComponentPtr>& components = Components();
        std::all_of(components.begin(), components.end(), [&](IComponentPtr component) -> bool {
            std::shared_ptr<ComponentType> concreteComponent = std::static_pointer_cast<ComponentType>(component);
            return func(concreteComponent);
        });
    }
    
    /*
     * Iterate over all components with casting component to a concrete type
     */
    void ForEachComponents(std::function<void(std::shared_ptr<ComponentType>)> func)
    {
        const std::set<IComponentPtr>& components = Components();
        std::for_each(components.begin(), components.end(), [&](IComponentPtr component) {
            std::shared_ptr<ComponentType> concreteComponent = std::static_pointer_cast<ComponentType>(component);
            func(concreteComponent);
        });
    }
    
    /*
     * Iterate over all dirty components and stop by condition with casting component to a concrete type
     */
    void AllOfDirtyComponents(std::function<bool(std::shared_ptr<ComponentType>)> func)
    {
        const std::set<IComponentPtr>& components = DirtyComponents();
        std::all_of(components.begin(), components.end(), [&](IComponentPtr component) -> bool {
            std::shared_ptr<ComponentType> concreteComponent = std::static_pointer_cast<ComponentType>(component);
            return func(concreteComponent);
        });
    }
    
    /*
     * Iterate over all dirty components with casting component to a concrete type
     */
    void ForEachDirtyComponents(std::function<void(std::shared_ptr<ComponentType>)> func)
    {
        const std::set<IComponentPtr>& components = DirtyComponents();
        std::for_each(components.begin(), components.end(), [&](IComponentPtr component) {
            std::shared_ptr<ComponentType> concreteComponent = std::static_pointer_cast<ComponentType>(component);
            func(concreteComponent);
        });
    }

private:
    std::set<IComponentPtr> m_Components;
    std::set<IComponentPtr> m_DirtyComponents;
    typeid_t m_RequiredComponent;
    std::thread::id m_MainThreadId;
};

}; // namespace jam

#endif /* ISYSTEM_H */
