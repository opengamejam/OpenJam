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
#include "CRenderComponent.h"
#include "CUpdateComponent.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

struct SOrderComparator {
    bool operator()(IEntityPtr e1, IEntityPtr e2) const
    {
        CRenderComponentPtr rc1 = e1->RenderComponent();
        CRenderComponentPtr rc2 = e1->RenderComponent();
        
        uint64_t o1 = OrderKey(rc1, e1);
        uint64_t o2 = OrderKey(rc2, e2);
        
#if defined(OS_KOS) // TODO: KOS render transparacy objects in other order
        if (rc2) {
            IMaterialPtr m2 = rc2->Material();
            if (m2 && !m2->Opacity()) {
                return o1 > o2;
            }
        }
#endif
        return o1 < o2;
    }
    
    INL static uint64_t OrderKey(CRenderComponentPtr rc, IEntityPtr e)
    {
        return (((uint64_t)e->HierarchyIndex() << 32) | (rc ? rc->DrawOrder() : 0));
    }
};

CEntityBase::CEntityBase()
    : m_HierarchyIndex(0)
{
}

CEntityBase::~CEntityBase()
{
}

void CEntityBase::Initialize(const std::string& name, const TComponentsArray& components)
{
    Name(name);

    m_Components.clear();
    std::for_each(components.begin(), components.end(), [&](const TComponentsArray::value_type& component) {
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
    assert("Cannot assign nullptr as component to entity" && component);
    if (!component) {
        return;
    }

    TComponentsArray& components = m_Components[component->GetId()];
    TComponentsArray::const_iterator it = std::find(components.begin(), components.end(), component);
    if (it == components.end()) {
        emit component->OnAddedSignal(component);
        
        IEntityPtr entity = Ptr<IEntity>();
        components.push_back(component);
        component->Entity(entity);
        
        if (component->GetId() == CTypeId<CTransformationComponent>::Id()) {
            m_TransformationComponent = std::static_pointer_cast<CTransformationComponent>(component);
        }
        if (component->GetId() == CTypeId<CRenderComponent>::Id()) {
            m_RenderComponent = std::static_pointer_cast<CRenderComponent>(component);
        }
        if (component->GetId() == CTypeId<CUpdateComponent>::Id()) {
            m_UpdateComponent = std::static_pointer_cast<CUpdateComponent>(component);
        }
    }
}

IComponentPtr CEntityBase::GetComponent(typeid_t id)
{
    TComponentsArray& components = m_Components[id];
    if (components.empty()) {
        return nullptr;
    }

    return components[0];
}

void CEntityBase::RemoveComponent(IComponentPtr component)
{
    assert("Cannot remove nullptr as component from entity" && component);
    if (!component) {
        return;
    }

    TComponentsArray& components = m_Components[component->GetId()];
    TComponentsArray::const_iterator it = std::find(components.begin(), components.end(), component);
    if (it != components.end()) {
        component->Entity(nullptr);
        components.erase(it);
        
        if (component->GetId() == CTypeId<CTransformationComponent>::Id()) {
            m_TransformationComponent = nullptr;
        }
        if (component->GetId() == CTypeId<CRenderComponent>::Id()) {
            m_RenderComponent = nullptr;
        }
        if (component->GetId() == CTypeId<CUpdateComponent>::Id()) {
            m_UpdateComponent = nullptr;
        }
        
        emit component->OnRemovedSignal(component);
    }
}

void CEntityBase::RemoveAllComponents(typeid_t id)
{
    TComponentsArray& components = m_Components[id];
    std::for_each(components.begin(), components.end(), [&](IComponentPtr component) {
        RemoveComponent(component);
    });
    components.clear();
}

bool CEntityBase::HasComponent(typeid_t id)
{
    TComponentsArray& components = m_Components[id];
    return !components.empty();
}

uint32_t CEntityBase::ComponentsNum(typeid_t id)
{
    TComponentsArray& components = m_Components[id];
    return static_cast<uint32_t>(components.size());
}

void CEntityBase::AddChild(IEntityPtr entity)
{
    assert("Cannot add nullptr as child to entity" && entity);
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

    std::shared_ptr<CEntityBase> baseEntity = entity->Ptr<CEntityBase>();

    baseEntity->Parent(Ptr<IEntity>());
    baseEntity->HierarchyIndex(HierarchyIndex() + 1);
    m_Children.push_back(entity);
    m_Children.sort(SOrderComparator());
}

void CEntityBase::RemoveChild(IEntityPtr entity)
{
    assert("Cannot remove nullptr as child from entity" && entity);
    if (!entity) {
        return;
    }
    
    IEntity::TEntitiesList::const_iterator it = std::find(m_Children.begin(), m_Children.end(), entity);
    if (it != m_Children.end()) {
        std::shared_ptr<CEntityBase> baseEntity = (*it)->Ptr<CEntityBase>();

        baseEntity->Parent(nullptr);
        baseEntity->HierarchyIndex(0);
        m_Children.erase(it);
    }
}

const IEntity::TEntitiesList& CEntityBase::Children() const
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

CUpdateComponentPtr CEntityBase::UpdateComponent() const
{
    return m_UpdateComponent;
}

void CEntityBase::Position(const glm::vec3& position)
{
    Get<CTransformationComponent, true>([position](CTransformationComponentPtr component) {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);
        transform.Position(position);
        component->AddTransform(CTransformationComponent::Local, transform);
    });
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
    Get<CTransformationComponent, true>([rotation](CTransformationComponentPtr component) {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);
        transform.Rotation(rotation);
        component->AddTransform(CTransformationComponent::Local, transform);
    });
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
    Get<CTransformationComponent, true>([scale](CTransformationComponentPtr component) {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);
        transform.Scale(scale);
        component->AddTransform(CTransformationComponent::Local, transform);
    });
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
    Get<CTransformationComponent, true>([anchorPoint](CTransformationComponentPtr component) {
        CTransform3Df transform = component->Transform(CTransformationComponent::Animation);
        transform.Position(anchorPoint);
        component->AddTransform(CTransformationComponent::Local, transform);
    });
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
    const TEntitiesList& childs = Children();
    std::for_each(childs.begin(), childs.end(), [&](IEntityPtr entity) {
        std::shared_ptr<CEntityBase> baseEntity = entity->Ptr<CEntityBase>();
        baseEntity->HierarchyIndex(HierarchyIndex() + 1);
    });
}

// *****************************************************************************
// Private Methods
// *****************************************************************************
