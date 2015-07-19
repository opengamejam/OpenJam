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
    
template<class T>
inline static std::type_index ComponentId()
{
    return std::type_index(typeid(T));
}

class IComponent : public IEventable, public std::enable_shared_from_this<IComponent>
{
public:
    IComponent(const std::type_index& id);
    virtual ~IComponent() = default;
    
    const std::type_index& Id() const;

    void Entity(IEntityWeak entityWeak);
    IEntityWeak Entity() const;
    
    void Dirty();
    
private:
    std::type_index m_CachedId;
    IEntityWeak m_Entity;
};

}; // namespace jam
    
#endif /* ICOMPONENT_H */
