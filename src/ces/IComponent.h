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

namespace jam {

CLASS_PTR(IComponent)
CLASS_PTR(IEntity)

/*
 * Interface IComponent
 */
class IComponent : public std::enable_shared_from_this<IComponent> {
    JAM_OBJECT_BASE
public:
    IComponent() = default;
    virtual ~IComponent() = default;

    virtual void Entity(IEntityPtr entityWeak) = 0;
    virtual IEntityPtr Entity() const = 0;

    virtual void Dirty() = 0;

signals:
    static CSignal<void, IEntityPtr> OnAddedSignal;
    static CSignal<void, IEntityPtr> OnRemovedSignal;
    static CSignal<void, IComponentPtr> OnChangedSignal;
};

/*
 * Base class CComponentBase
 */
class CComponentBase : public IComponent {
    JAM_OBJECT
public:
    CComponentBase() = default;
    virtual ~CComponentBase() = default;

    void Entity(IEntityPtr entityWeak) override;
    IEntityPtr Entity() const override;

    virtual void Dirty() override;

private:
    IEntityWeak m_Entity;
};

}; // namespace jam

#endif /* ICOMPONENT_H */
