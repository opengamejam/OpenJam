//
//  IComponent.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include "CSignal.hpp"
#include "IObject.h"

namespace jam {

CLASS_PTR(IComponent)
CLASS_PTR(IEntity)

/*
 * Interface IComponent
 */
class IComponent : public IObject<IComponent> {
public:
    /*
     * Constructor
     */
    IComponent() = default;
    
    /*
     * Destructor
     */
    virtual ~IComponent() = default;

    /*
     * Assign entity to component, if it already has entity then will send remove component signal
     * for this entity and then send add component for new
     */
    virtual void Entity(IEntityPtr entity) = 0;
    
    /*
     * Returns assigned entity
     */
    virtual IEntityPtr Entity() const = 0;

    /*
     * Mark component as dirty. It will automatically notify systems that this component was changed
     */
    virtual void Dirty() = 0;

signals:
    /*
     * Signal to notify that component was added to entity. Used by systems
     */
    static CSignal<void, IComponentPtr> OnAddedSignal;
    
    /*
     * Signal to notify that component was removed from entity. Used by systems
     */
    static CSignal<void, IComponentPtr> OnRemovedSignal;
    
    /*
     * Signal to notify that component was changed. Used by systems
     */
    static CSignal<void, IComponentPtr> OnChangedSignal;
};

/*
 * Base class CComponentBase
 */
class CComponentBase : public IComponent {
    JAM_OBJECT
public:
    /*
     * Constructor
     */
    CComponentBase() = default;
    
    /*
     * Destructor
     */
    virtual ~CComponentBase() = default;

    /*
     * Assign entity to component, if it already has entity then will send remove component signal
     * for this entity and then send add component for new
     */
    void Entity(IEntityPtr entity) override;
    
    /*
     * Returns assigned entity
     */
    IEntityPtr Entity() const override;

    /*
     * Mark component as dirty. It will automatically notify systems that this component was changed
     */
    virtual void Dirty() override;

private:
    IEntityWeak m_Entity;
};

}; // namespace jam

#endif /* ICOMPONENT_H */
