//
//  IComponent.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include "IEventable.h"

namespace jam
{

CLASS_PTR(IComponent);
CLASS_WEAK(IEntity);

class IComponent : public IEventable, public std::enable_shared_from_this<IComponent>
{
    JAM_OBJECT_BASE
public:
    IComponent();
    virtual ~IComponent() = default;

    void Entity(IEntityWeak entityWeak);
    IEntityWeak Entity() const;
    
    void Dirty();
    
private:
    IEntityWeak m_Entity;
};

}; // namespace jam
    
#endif /* ICOMPONENT_H */
