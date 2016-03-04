//
//  CTransformAffector.cpp
//  TestApp
//
//  Created by yev on 8/7/15.
//
//

#include "CTransformAffector.h"
#include "IEntity.h"
#include "CTransformationComponent.h"

using namespace jam;

void CTransformAffector::Translating(IEntityPtr entity, const glm::vec3& translation)
{
    entity->Get<CTransformationComponent>([translation](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);

        glm::vec3 value = transform.Position();
        value += translation;
        transform.Position(value);

        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    });
}

void CTransformAffector::Rotating(IEntityPtr entity, const glm::vec3& rotation)
{
    entity->Get<CTransformationComponent>([rotation](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);

        glm::vec3 value = transform.Rotation();
        value += rotation;
        transform.Rotation(value);

        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    });
}

void CTransformAffector::Scaling(IEntityPtr entity, const glm::vec3& scaling)
{
    entity->Get<CTransformationComponent>([scaling](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);

        glm::vec3 value = transform.Scale();
        value += scaling;
        transform.Scale(value);

        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    });
}

void CTransformAffector::Position(IEntityPtr entity, const glm::vec3& position)
{
    entity->Get<CTransformationComponent>([position](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);

        transform.Position(position);

        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    });
}

void CTransformAffector::Rotation(IEntityPtr entity, const glm::vec3& rotation)
{
    entity->Get<CTransformationComponent>([rotation](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);

        transform.Rotation(rotation);

        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    });
}

void CTransformAffector::Scale(IEntityPtr entity, const glm::vec3& scaling)
{
    entity->Get<CTransformationComponent>([scaling](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);

        transform.Scale(scaling);

        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    });
}

glm::vec3 CTransformAffector::Position(IEntityPtr entity)
{
    glm::vec3 value;
    entity->Get<CTransformationComponent>([&](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);
        value = transform.Position();
    });

    return value;
}

glm::vec3 CTransformAffector::Rotation(IEntityPtr entity)
{
    glm::vec3 value;
    entity->Get<CTransformationComponent>([&](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);
        value = transform.Rotation();
    });

    return value;
}

glm::vec3 CTransformAffector::Scale(IEntityPtr entity)
{
    glm::vec3 value;
    entity->Get<CTransformationComponent>([&](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);
        value = transform.Scale();
    });

    return value;
}

void CTransformAffector::Translating(CTransformationComponentPtr component, const glm::vec3& translation)
{
    if (!component)
    {
        return;
    }

    CTransform3Df transform = component->Transform(CTransformationComponent::Local);

    glm::vec3 value = transform.Position();
    value += translation;
    transform.Position(value);

    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

void CTransformAffector::Rotating(CTransformationComponentPtr component, const glm::vec3& rotation)
{
    if (!component)
    {
        return;
    }

    CTransform3Df transform = component->Transform(CTransformationComponent::Local);

    glm::vec3 value = transform.Rotation();
    value += rotation;
    transform.Rotation(value);

    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

void CTransformAffector::Scaling(CTransformationComponentPtr component, const glm::vec3& scaling)
{
    if (!component)
    {
        return;
    }

    CTransform3Df transform = component->Transform(CTransformationComponent::Local);

    glm::vec3 value = transform.Scale();
    value += scaling;
    transform.Scale(value);

    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

void CTransformAffector::Position(CTransformationComponentPtr component, const glm::vec3& position)
{
    if (!component)
    {
        return;
    }

    CTransform3Df transform = component->Transform(CTransformationComponent::Local);

    transform.Position(position);

    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

void CTransformAffector::Rotation(CTransformationComponentPtr component, const glm::vec3& rotation)
{
    if (!component)
    {
        return;
    }

    CTransform3Df transform = component->Transform(CTransformationComponent::Local);

    transform.Rotation(rotation);

    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

void CTransformAffector::Scale(CTransformationComponentPtr component, const glm::vec3& scaling)
{
    if (!component)
    {
        return;
    }

    CTransform3Df transform = component->Transform(CTransformationComponent::Local);

    transform.Scale(scaling);

    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}
