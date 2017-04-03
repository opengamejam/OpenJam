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
CLASS_PTR(CRenderComponent)
CLASS_PTR(CTransformationComponent)

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
    /*
     * Constructor
     */
    IEntity() = default;
    
    /*
     * Destructor
     */
    virtual ~IEntity() = default;

    /*
     * Initialize entity with 'name' and list of components
     */
    virtual void Initialize(const std::string& name, const TComponentsList& components) = 0;

    /*
     * Get entity name
     */
    virtual const std::string& Name() const = 0;
    
    /*
     * Set entity name
     */
    virtual void Name(const std::string& name) = 0;

    /*
     * Add component to entity
     */
    virtual void AddComponent(IComponentPtr component) = 0;
    
    /*
     * Get component from entity by typeid_t. Returns nullptr if component doesn't added to entity
     */
    virtual IComponentPtr GetComponent(typeid_t id) = 0;

    /*
     * Get component from entity by template parameter. Returns nullptr if component doesn't added to entity
     */
    template <class T> // TODO: enable_if IComponent
    std::shared_ptr<T> Get()
    {
        typeid_t id = CTypeId<T>::Id();
        return std::static_pointer_cast<T>(GetComponent(id));
    }

    /*
     * Get component from entity by template parameter. If component exists lambda function will be called with 
     * this component in parameter
     */
    template <class T>
    void Get(std::function<void(std::shared_ptr<T>)> func)
    {
        std::shared_ptr<T> component = Get<T>();
        if (component && func) {
            func(component);
        }
    }

    /*
     * Remove component from entity
     */
    virtual void RemoveComponent(IComponentPtr component) = 0;
    
    /*
     * Remove all components from entity
     */
    virtual void RemoveAllComponents(typeid_t id) = 0;
    
    /*
     * Check if component with this typeid_t added to entity
     */
    virtual bool HasComponent(typeid_t id) = 0;
    
    /*
     * Get numbers fo added to entity components
     */
    virtual uint32_t ComponentsNum(typeid_t id) = 0;

    /*
     * Add child entity
     */
    virtual void AddChild(IEntityPtr entity) = 0;
    
    /*
     * Remove child entity
     */
    virtual void RemoveChild(IEntityPtr entity) = 0;
    
    /*
     * Get children entities
     */
    virtual const TEntities& Children() const = 0;

    /*
     * Get parent entity
     */
    virtual IEntityPtr Parent() const = 0;

    /*
     * Global z-index
     */
    virtual uint32_t HierarchyIndex() const = 0;
    
    /* GameObject functionality */
    
    /*
     * Get render component
     */
    virtual CRenderComponentPtr RenderComponent() const = 0;
    
    /*
     * Assign render component
     */
    virtual CTransformationComponentPtr TransformationComponent() const = 0;
    
    /*
     * Set entity position
     */
    virtual void Position(const glm::vec3& position) = 0;
    
    /*
     * Get entity position
     */
    virtual const glm::vec3& Position() = 0;
    
    /*
     * Set entity rotation
     */
    virtual void Rotation(const glm::vec3& rotation) = 0;
    
    /*
     * Get entity rotation
     */
    virtual const glm::vec3& Rotation() = 0;
    
    /*
     * Set entity scale
     */
    virtual void Scale(const glm::vec3& scale) = 0;
    
    /*
     * Get entity scale
     */
    virtual const glm::vec3& Scale() = 0;
    
    /*
     * Set entity anchor point
     */
    virtual void AnchorPoint(const glm::vec3& anchorPoint) = 0;
    
    /*
     * Get entity anchor point
     */
    virtual const glm::vec3& AnchorPoint() = 0;
};

/*
 * Base class CEntityBase
 */
class CEntityBase : public IEntity {
    JAM_OBJECT
public:
    /*
     * Constructor
     */
    CEntityBase();
    
    /*
     * Destructor
     */
    virtual ~CEntityBase();

    /*
     * Initialize entity with 'name' and list of components
     */
    virtual void Initialize(const std::string& name, const TComponentsList& components) override;
    
    /*
     * Get entity name
     */
    virtual const std::string& Name() const override;
    
    /*
     * Set entity name
     */
    virtual void Name(const std::string& name) override;
    
    /*
     * Add component to entity
     */
    virtual void AddComponent(IComponentPtr component) override;
    
    /*
     * Get component from entity by typeid_t. Returns nullptr if component doesn't added to entity
     */
    virtual IComponentPtr GetComponent(typeid_t id) override;
    
    /*
     * Remove component from entity
     */
    virtual void RemoveComponent(IComponentPtr component) override;
    
    /*
     * Remove all components from entity
     */
    virtual void RemoveAllComponents(typeid_t id) override;
    
    /*
     * Check if component with this typeid_t added to entity
     */
    virtual bool HasComponent(typeid_t id) override;
    
    /*
     * Get numbers fo added to entity components
     */
    virtual uint32_t ComponentsNum(typeid_t id) override;
    
    /*
     * Add child entity
     */
    virtual void AddChild(IEntityPtr entity) override;
    
    /*
     * Remove child entity
     */
    virtual void RemoveChild(IEntityPtr entity) override;
    
    /*
     * Get children entities
     */
    virtual const TEntities& Children() const override;
    
    /*
     * Get parent entity
     */
    virtual IEntityPtr Parent() const override;
    
    /*
     * Global z-index
     */
    virtual uint32_t HierarchyIndex() const override;
    
    /* GameObject functionality */
    
    /*
     * Get render component
     */
    virtual CRenderComponentPtr RenderComponent() const override;
    
    /*
     * Assign render component
     */
    virtual CTransformationComponentPtr TransformationComponent() const override;
    
    /*
     * Set entity position
     */
    virtual void Position(const glm::vec3& position) override;
    
    /*
     * Get entity position
     */
    virtual const glm::vec3& Position() override;
    
    /*
     * Set entity rotation
     */
    virtual void Rotation(const glm::vec3& rotation) override;
    
    /*
     * Get entity rotation
     */
    virtual const glm::vec3& Rotation() override;
    
    /*
     * Set entity scale
     */
    virtual void Scale(const glm::vec3& scale) override;
    
    /*
     * Get entity scale
     */
    virtual const glm::vec3& Scale() override;
    
    /*
     * Set entity anchor point
     */
    virtual void AnchorPoint(const glm::vec3& anchorPoint) override;
    
    /*
     * Get entity anchor point
     */
    virtual const glm::vec3& AnchorPoint() override;

protected:
    void Parent(IEntityPtr parent);
    void HierarchyIndex(uint32_t hierarchyIndex);

private:
    std::string m_Name;
    TComponentsMap m_Components;
    TEntities m_Children;
    IEntityWeak m_Parent;
    uint32_t m_HierarchyIndex;
    
    CRenderComponentPtr m_RenderComponent;
    CTransformationComponentPtr m_TransformationComponent;
};

}; // namespace jam

#endif /* IENTITY_H */

