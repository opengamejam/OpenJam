//
//  CRenderSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CRenderSystem.h"
#include "IEntity.h"
#include "ICamera.h"
#include "CRenderComponent.h"
#include "CTransformationComponent.h"
#include "CBatchComponent.h"
#include "RenderGlobal.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

struct SOrderComparator
{
    bool operator()(CRenderComponentPtr rc1, CRenderComponentPtr rc2) const
    {
        IEntityPtr e1 = rc1->Entity().lock();
        IEntityPtr e2 = rc2->Entity().lock();
        if (!e1 || !e2)
        {
            return false;
        }

        uint64_t o1 = OrderKey(rc1, e1);
        uint64_t o2 = OrderKey(rc2, e2);

#if defined(OS_KOS) // TODO: KOS render transparacy objects in other order
        IMaterialPtr m2 = rc2->Material();
        if (m2 && !m2->Opacity())
        {
        	return o1 > o2;
        }
#endif
        return o1 < o2;
    }
    
    INL static uint64_t OrderKey(CRenderComponentPtr rc, IEntityPtr e)
    {
        return (((uint64_t)e->HierarchyIndex() << 32) | rc->DrawOrder());
    }
};

CRenderSystem::CRenderSystem(IRendererPtr renderer)
    : m_Renderer(renderer)
{
    assert(m_Renderer);
    RegisterComponent<CRenderComponent>();
}

CRenderSystem::~CRenderSystem()
{
    
}

void CRenderSystem::Update(unsigned long dt)
{
    /*const ISystem::TEntities& entities = Entities();
    std::for_each(entities.begin(), entities.end(), [&](IEntityPtr entity)
    {
        CTransformationComponentPtr transformComponent = entity->Get<CTransformationComponent>();
        
        entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComponent)
        {
            if (!IsEntityAdded(entity))
            {
                return;
            }
        });
    });*/
    
    ClearDirtyEntities();
    m_ProccededRenderTargets.clear();
}

void CRenderSystem::Draw(ICameraPtr camera)
{
    if (!camera)
    {
        return;
    }
    
    IRenderTargetPtr currentRenderTarget = camera->RenderTarget();
    if (!currentRenderTarget)
    {
        return;
    }
    
    currentRenderTarget->Bind();
    if (m_ProccededRenderTargets.find(currentRenderTarget) == m_ProccededRenderTargets.end())
    {
        currentRenderTarget->Clear();
        m_ProccededRenderTargets.insert(currentRenderTarget);
    }
    
    glm::mat4 projectionMatrix = camera->ProjectionMatrix();
    glm::mat4 viewMatrix(1.0);
    camera->Get<CTransformationComponent>([&](CTransformationComponentPtr viewTransform)
    {
        viewMatrix = viewTransform->ResultTransform()();
    });
    
    // Draw
    unsigned int cameraId = camera->Id();
    std::for_each(m_SortedComponents.begin(), m_SortedComponents.end(), [&](CRenderComponentPtr renderComponent)
    {
        if (!renderComponent->HasCameraId(cameraId))
        {
            return;
        }
        
        if (renderComponent->Batchable())
        {
            IMeshPtr mesh = renderComponent->Mesh(CRenderComponent::kBatchingGroupName);
            if (m_ProccededBatches.find(mesh->UniqueId()) != m_ProccededBatches.end())
            {
                return;
            }
            
            DrawGroup(renderComponent, CRenderComponent::kBatchingGroupName, projectionMatrix, viewMatrix, glm::mat4(1.0));
            m_ProccededBatches.insert(mesh->UniqueId());
        }
        else
        {
            IEntityPtr entity = renderComponent->Entity().lock();
            glm::mat4 modelMatrix(1.0);
            if (entity)
            {
                entity->Get<CTransformationComponent>([&](CTransformationComponentPtr modelTransform)
                {
                    modelMatrix = modelTransform->ResultTransform()();
                });
            }
            
            const std::set<std::string>& groups = renderComponent->Groups();
            std::for_each(groups.begin(), groups.end(), [&](const std::string& groupName)
            {
                if (groupName == CRenderComponent::kBatchingGroupName)
                {
                    return;
                }
                
                DrawGroup(renderComponent, groupName, projectionMatrix, viewMatrix, modelMatrix);
            });
        }
    });
    
    currentRenderTarget->Unbind();
    m_ProccededBatches.clear();
}

void CRenderSystem::DrawGroup(CRenderComponentPtr renderComponent,
                              const std::string& groupName,
                              const glm::mat4& projectionMatrix,
                              const glm::mat4& viewMatrix,
                              const glm::mat4& modelMatrix) const
{
    IMeshPtr mesh = renderComponent->Mesh(groupName);
    IMaterialPtr material = renderComponent->Material(groupName);
    ITexturePtr texture = renderComponent->Texture(groupName);
    IShaderProgramPtr shader = renderComponent->Shader(groupName);
    if (!shader || !material || !mesh)
    {
        return;
    }
    
    glm::mat4 normalMat = glm::transpose(glm::inverse(viewMatrix * modelMatrix));
    shader->BindUniformMatrix4x4f("MainNormalMatrix", normalMat);
    shader->BindUniformMatrix4x4f("MainProjectionMatrix", projectionMatrix);
    shader->BindUniformMatrix4x4f("MainViewMatrix", viewMatrix);
    
    Draw(mesh, material, texture, shader);
}

void CRenderSystem::Draw(IMeshPtr mesh, IMaterialPtr material, ITexturePtr texture, IShaderProgramPtr shader) const
{
    shader->Bind();
    shader->UpdateUniforms();
    material->Bind();
    if (texture)
    {
        texture->Bind();
    }
    mesh->Bind();
    
    m_Renderer->Draw(mesh, material, shader);
    
    mesh->Unbind();
    if (texture)
    {
        texture->Unbind();
    }
    material->Unbind();
    shader->Unbind();
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CRenderSystem::OnAddedEntity(IEntityPtr entity)
{
    ISystem::OnAddedEntity(entity);
    
    entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComponent)
    {
        std::map<CRenderComponentPtr, uint64_t>::const_iterator it = m_OrderKeys.find(renderComponent);
        if (it != m_OrderKeys.end())
        {
            return;
        }
        
        uint64_t order_key = SOrderComparator::OrderKey(renderComponent, entity);
        
        m_OrderKeys[renderComponent] = order_key;
        m_SortedComponents.push_back(renderComponent);
        m_SortedComponents.sort(SOrderComparator());
    });
}

void CRenderSystem::OnChangedComponent(IComponentPtr component)
{
    ISystem::OnChangedComponent(component);
    
    CRenderComponentPtr renderComponent = std::static_pointer_cast<CRenderComponent>(component);
    
    std::map<CRenderComponentPtr, uint64_t>::iterator it = m_OrderKeys.find(renderComponent);
    if (it == m_OrderKeys.end())
    {
        return;
    }
    
    uint64_t order_key = SOrderComparator::OrderKey(renderComponent, renderComponent->Entity().lock());
    if (order_key != it->second)
    {
        it->second = order_key;
        m_SortedComponents.sort(SOrderComparator());
    }
}

void CRenderSystem::OnRemovedEntity(IEntityPtr entity)
{
    ISystem::OnRemovedEntity(entity);
    
    entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComponent)
    {
        if (m_OrderKeys.find(renderComponent) == m_OrderKeys.end())
        {
            return;
        }
        
        m_OrderKeys.erase(renderComponent);
        std::list<CRenderComponentPtr>::const_iterator it = std::find(m_SortedComponents.begin(), m_SortedComponents.end(), renderComponent);
        m_SortedComponents.erase(it);
    });
}
