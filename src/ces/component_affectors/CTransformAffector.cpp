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

void CTransformAffector::Translating(IEntityPtr entity, const CVector3Df& translation)
{
    entity->Get<CTransformationComponent>([translation](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);
        
        CVector3Df value = transform.Position();
        value += translation;
        transform.Position(value);
        
        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    });
}

void CTransformAffector::Rotating(IEntityPtr entity, const CVector3Df& rotation)
{
    entity->Get<CTransformationComponent>([rotation](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);

        CVector3Df value = transform.Rotation();
        value += rotation;
        transform.Rotation(value);

        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    });
}

void CTransformAffector::Scaling(IEntityPtr entity, const CVector3Df& scaling)
{
    entity->Get<CTransformationComponent>([scaling](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);

        CVector3Df value = transform.Scale();
        value += scaling;
        transform.Scale(value);

        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    });
}

void CTransformAffector::Position(IEntityPtr entity, const CVector3Df& position)
{
    entity->Get<CTransformationComponent>([position](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);

        transform.Position(position);

        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    });
}

void CTransformAffector::Rotation(IEntityPtr entity, const CVector3Df& rotation)
{
    entity->Get<CTransformationComponent>([rotation](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);

        transform.Rotation(rotation);

        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    });
}

void CTransformAffector::Scale(IEntityPtr entity, const CVector3Df& scaling)
{
    entity->Get<CTransformationComponent>([scaling](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);

        transform.Scale(scaling);

        component->AddTransform(CTransformationComponent::Local, transform);
        component->Dirty();
    });
}

CVector3Df CTransformAffector::Position(IEntityPtr entity)
{
    CVector3Df value;
    entity->Get<CTransformationComponent>([&](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);
        value = transform.Position();
    });
    
    return value;
}

CVector3Df CTransformAffector::Rotation(IEntityPtr entity)
{
    CVector3Df value;
    entity->Get<CTransformationComponent>([&](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);
        value = transform.Rotation();
    });
    
    return value;
}

CVector3Df CTransformAffector::Scale(IEntityPtr entity)
{
    CVector3Df value;
    entity->Get<CTransformationComponent>([&](CTransformationComponentPtr component)
    {
        CTransform3Df transform = component->Transform(CTransformationComponent::Local);
        value = transform.Scale();
    });
    
    return value;
}

void CTransformAffector::Translating(CTransformationComponentPtr component, const CVector3Df& translation)
{
    if (!component)
    {
        return;
    }
    
    CTransform3Df transform = component->Transform(CTransformationComponent::Local);
    
    CVector3Df value = transform.Position();
    value += translation;
    transform.Position(value);
    
    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

void CTransformAffector::Rotating(CTransformationComponentPtr component, const CVector3Df& rotation)
{
    if (!component)
    {
        return;
    }
    
    CTransform3Df transform = component->Transform(CTransformationComponent::Local);
    
    CVector3Df value = transform.Rotation();
    value += rotation;
    transform.Rotation(value);
    
    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

void CTransformAffector::Scaling(CTransformationComponentPtr component, const CVector3Df& scaling)
{
    if (!component)
    {
        return;
    }
    
    CTransform3Df transform = component->Transform(CTransformationComponent::Local);
    
    CVector3Df value = transform.Scale();
    value += scaling;
    transform.Scale(value);
    
    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

void CTransformAffector::Position(CTransformationComponentPtr component, const CVector3Df& position)
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

void CTransformAffector::Rotation(CTransformationComponentPtr component, const CVector3Df& rotation)
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

void CTransformAffector::Scale(CTransformationComponentPtr component, const CVector3Df& scaling)
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