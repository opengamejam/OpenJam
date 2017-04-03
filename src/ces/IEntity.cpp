//
//  IEntity.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "IEntity.h"
#include "CRenderComponent.h"
#include "CTransformationComponent.h"


using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CEntityBase::CEntityBase()
    : m_HierarchyIndex(0)
{
}

CEntityBase::~CEntityBase()
{
}

void CEntityBase::Initialize(const std::string& name, const TComponentsList& components)
{
    Name(name);

    m_Components.clear();
    std::for_each(components.begin(), components.end(), [&](const TComponentsList::value_type& component) {
        AddComponent(component);
    });
}

const std::string& CEntityBase::Name() const
{
    return m_Name;
}

void CEntityBase::Name(const std::string& name)
{
    m_Name = name;
}

void CEntityBase::AddComponent(IComponentPtr component)
{
    if (!component) {
        return;
    }

    TComponentsList& components = m_Components[component->GetId()];
    TComponentsList::const_iterator it = std::find(components.begin(), components.end(), component);
    if (it == components.end()) {
        IEntityPtr entity = shared_from_this();

        components.push_back(component);
        component->Entity(entity);
        
        if (component->GetId() == CTypeId<CTransformationComponent>::Id()) {
            m_TransformationComponent = std::static_pointer_cast<CTransformationComponent>(component);
        }
        if (component->GetId() == CTypeId<CRenderComponent>::Id()) {
            m_RenderComponent = std::static_pointer_cast<CRenderComponent>(component);
        }

        emit component->OnAddedSignal(entity);
    }
}

IComponentPtr CEntityBase::GetComponent(typeid_t id)
{
    TComponentsList& components = m_Components[id];
    if (components.empty()) {
        return nullptr;
    }

    return components[0];
}

void CEntityBase::RemoveComponent(IComponentPtr component)
{
    if (!component) {
        return;
    }

    TComponentsList& components = m_Components[component->GetId()];
    TComponentsList::const_iterator it = std::find(components.begin(), components.end(), component);
    if (it != components.end()) {
        IEntityPtr entity = shared_from_this();
        emit component->OnAddedSignal(entity);

        component->Entity(nullptr);
        components.erase(it);
        
        if (component->GetId() == CTypeId<CTransformationComponent>::Id()) {
            m_TransformationComponent = nullptr;
        }
        if (component->GetId() == CTypeId<CRenderComponent>::Id()) {
            m_RenderComponent = nullptr;
        }
    }
}

void CEntityBase::RemoveAllComponents(typeid_t id)
{
    TComponentsList& components = m_Components[id];
    std::for_each(components.begin(), components.end(), [&](IComponentPtr component) {
        RemoveComponent(component);
    });
    components.clear();
}

bool CEntityBase::HasComponent(typeid_t id)
{
    TComponentsList& components = m_Components[id];
    return !components.empty();
}

uint32_t CEntityBase::ComponentsNum(typeid_t id)
{
    TComponentsList& components = m_Components[id];
    return static_cast<uint32_t>(components.size());
}

void CEntityBase::AddChild(IEntityPtr entity)
{
    if (!entity) {
        return;
    }

    IEntityPtr prevParent = entity->Parent();
    assert(prevParent != entity);
    if (prevParent == entity && prevParent != nullptr) {
        return;
    }

    if (prevParent) {
        prevParent->RemoveChild(entity);
    }

    std::shared_ptr<CEntityBase> baseEntity = std::static_pointer_cast<CEntityBase>(entity);

    baseEntity->Parent(shared_from_this());
    baseEntity->HierarchyIndex(HierarchyIndex() + 1);
    m_Children.push_back(entity);

    std::for_each(baseEntity->m_Components.begin(), baseEntity->m_Components.end(),
        [entity](const TComponentsMap::value_type& element) {
            const TComponentsList& components = element.second;
            std::for_each(components.begin(), components.end(), [entity](const TComponentsList::value_type& component) {
                emit component->OnAddedSignal(entity);
            });
        });
}

void CEntityBase::RemoveChild(IEntityPtr entity)
{
    IEntity::TEntities::const_iterator it = std::find(m_Children.begin(), m_Children.end(), entity);
    if (it != m_Children.end()) {
        std::shared_ptr<CEntityBase> baseEntity = std::static_pointer_cast<CEntityBase>(*it);

        baseEntity->Parent(nullptr);
        baseEntity->HierarchyIndex(0);
        m_Children.erase(it);
    }

    std::for_each(m_Components.begin(), m_Components.end(), [entity](const TComponentsMap::value_type& element) {
        const TComponentsList& components = element.second;
        std::for_each(components.begin(), components.end(), [entity](const TComponentsList::value_type& component) {
            emit component->OnRemovedSignal(entity);
        });
    });
}

const IEntity::TEntities& CEntityBase::Children() const
{
    return m_Children;
}

IEntityPtr CEntityBase::Parent() const
{
    return m_Parent.lock();
}

uint32_t CEntityBase::HierarchyIndex() const
{
    return m_HierarchyIndex;
}

CRenderComponentPtr CEntityBase::RenderComponent() const
{
    return m_RenderComponent;
}

CTransformationComponentPtr CEntityBase::TransformationComponent() const
{
    return m_TransformationComponent;
}

void CEntityBase::Position(const glm::vec3& position)
{
    CTransformationComponentPtr component = TransformationComponent();
    if (component) {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);
        transform.Position(position);
        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    }
}

const glm::vec3& CEntityBase::Position()
{
    CTransformationComponentPtr component = TransformationComponent();
    if (component) {
        const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
        return transform.Position();
    }
    static glm::vec3 emptyValue;
    emptyValue = glm::vec3();
    return emptyValue;
}

void CEntityBase::Rotation(const glm::vec3& rotation)
{
    CTransformationComponentPtr component = TransformationComponent();
    if (component) {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);
        transform.Rotation(rotation);
        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    }
}

const glm::vec3& CEntityBase::Rotation()
{
    CTransformationComponentPtr component = TransformationComponent();
    if (component) {
        const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
        return transform.Rotation();
    }
    static glm::vec3 emptyValue;
    emptyValue = glm::vec3();
    return emptyValue;
}

void CEntityBase::Scale(const glm::vec3& scale)
{
    CTransformationComponentPtr component = TransformationComponent();
    if (component) {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);
        transform.Scale(scale);
        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    }
}

const glm::vec3& CEntityBase::Scale()
{
    CTransformationComponentPtr component = TransformationComponent();
    if (component) {
        const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
        return transform.Scale();
    }
    static glm::vec3 emptyValue;
    emptyValue = glm::vec3();
    return emptyValue;
}

void CEntityBase::AnchorPoint(const glm::vec3& anchorPoint)
{
    CTransformationComponentPtr component = TransformationComponent();
    if (component) {
        CTransform3Df transform = component->Transform(CTransformationComponent::Animation);
        transform.Position(anchorPoint);
        component->AddTransform(CTransformationComponent::Animation, transform);
        component->Dirty();
    }
}

const glm::vec3& CEntityBase::AnchorPoint()
{
    CTransformationComponentPtr component = TransformationComponent();
    if (component) {
        const CTransform3Df& transform = component->Transform(CTransformationComponent::Animation);
        return transform.Position();
    }
    static glm::vec3 emptyValue;
    emptyValue = glm::vec3();
    return emptyValue;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CEntityBase::Parent(IEntityPtr parent)
{
    m_Parent = parent;
}

void CEntityBase::HierarchyIndex(uint32_t hierarchyIndex) // TODO: Optimization
{
    m_HierarchyIndex = hierarchyIndex;
    const TEntities& childs = Children();
    std::for_each(childs.begin(), childs.end(), [&](IEntityPtr entity) {
        std::shared_ptr<CEntityBase> baseEntity = std::static_pointer_cast<CEntityBase>(entity);
        baseEntity->HierarchyIndex(HierarchyIndex() + 1);
    });
}

// *****************************************************************************
// Private Methods
// *****************************************************************************
