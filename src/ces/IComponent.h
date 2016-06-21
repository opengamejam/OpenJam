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

namespace jam
{

CLASS_PTR(IComponent);
CLASS_PTR(IEntity);
CLASS_WEAK(IEntity);

class IComponent : public std::enable_shared_from_this<IComponent>
{
    JAM_OBJECT_BASE
    
    friend class ISystem;
public:
    IComponent();
    virtual ~IComponent() = default;

    void Entity(IEntityWeak entityWeak);
    IEntityWeak Entity() const;
    
    void Dirty();
    
signals:
    static CSignal<void, IEntityPtr> OnAddedSignal;
    static CSignal<void, IEntityPtr> OnRemovedSignal;
    static CSignal<void, IComponentPtr> OnChangedSignal;

private:
    IEntityWeak m_Entity;
};

}; // namespace jam
    
#endif /* ICOMPONENT_H */
