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
#include "RenderGlobal.h"
#include "CBatch.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

struct SOrderComparator
{
    bool operator()(CRenderComponentPtr re1, CRenderComponentPtr re2) const
    {
        IEntityPtr e1 = re1->Entity().lock();
        IEntityPtr e2 = re2->Entity().lock();
        if (!e1 || !e2)
        {
            return false;
        }

        uint64_t o1 = OrderKey(re1, e1);
        uint64_t o2 = OrderKey(re2, e2);

#if defined(OS_KOS) // TODO: KOS renders transparacy objects in other order
        IMaterialPtr m2 = re2->Material();
        if (m2 && !m2->Opacity())
        {
        	return o1 > o2;
        }
#endif
        return o1 < o2;
    }
    
    INL static uint64_t OrderKey(CRenderComponentPtr re, IEntityPtr e)
    {
        return (((uint64_t)e->HierarchyIndex() << 32) | re->DrawOrder());
    }
};

CRenderSystem::CRenderSystem()
{
    RegisterComponent(ComponentId<CRenderComponent>());
    
    batch.reset(new CBatch());
}

CRenderSystem::~CRenderSystem()
{
    
}

void CRenderSystem::Update(unsigned long dt)
{
    const ISystem::TEntities& entities = Entities();
    std::for_each(entities.begin(), entities.end(), [&](IEntityPtr entity)
    {
        CTransformationComponentPtr transformComponent = entity->Get<CTransformationComponent>();
        
        entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComponent)
        {
            if (!IsEntityAdded(entity))
            {
                return;
            }
            
            if (renderComponent->Batchable())
            {
                const std::set<std::string>& groups = renderComponent->Groups();
                std::for_each(groups.begin(), groups.end(), [&](const std::string& groupName)
                {
                    IMeshPtr mesh = renderComponent->Mesh(groupName);
                    IMaterialPtr material = renderComponent->Material(groupName);
                    ITexturePtr texture = renderComponent->Texture(groupName);
                    IShaderProgramPtr shader = renderComponent->Shader(groupName);
                    
                    if (!batch->IsInitialized())
                    {
                        batch->Initialize(material, shader, {texture},
                                          mesh->VertexBuffer()->ElementSize(),
                                          mesh->IndexBuffer() ? mesh->IndexBuffer()->ElementSize() : 0);
                    }
                    
                    CTransform3Df transform = (transformComponent ? transformComponent->ResultTransform() : CTransform3Df());
                    batch->AddGeometry(mesh, transform);
                });
            }
            else
            {
                
            }
        });
    });
    
    batch->Update();
    
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
    
    // Draw
    unsigned int cameraId = camera->Id();
    std::for_each(m_SortedComponents.begin(), m_SortedComponents.end(), [&](CRenderComponentPtr renderComponent)
    {
        if (!renderComponent->HasCameraId(cameraId) ||
            renderComponent->Batchable())
        {
            return;
        }

		const std::set<std::string>& groups = renderComponent->Groups();
		std::for_each(groups.begin(), groups.end(), [&](const std::string& groupName)
		{
			IMeshPtr mesh = renderComponent->Mesh(groupName);
			IMaterialPtr material = renderComponent->Material(groupName);
			ITexturePtr texture = renderComponent->Texture(groupName);
			IShaderProgramPtr shader = renderComponent->Shader(groupName);

			if (!shader || !material || !mesh)
			{
				return;
			}

			shader->Bind();
			material->Bind();
			if (texture)
			{
				texture->Bind();
			}
			mesh->Bind();
            // TODO: Move it to another place 
			shader->BindUniformMatrix4x4f("MainProjectionMatrix", camera->ProjectionMatrix());
			shader->UpdateUniforms();

			GRenderer->Draw(mesh, material, shader);

			mesh->Unbind();
			if (texture)
			{
                texture->Unbind();
			}
			material->Unbind();
			shader->Unbind();
		});
    });
    
    if (batch->IsInitialized())
    {
        IMeshPtr mesh = batch->Mesh();
        IMaterialPtr material = batch->Material();
        ITexturePtr texture = batch->Textures().front();
        IShaderProgramPtr shader = batch->Shader();
        
        if (!shader || !material || !mesh)
        {
            return;
        }
        
        shader->Bind();
        material->Bind();
        if (texture)
        {
            texture->Bind();
        }
        mesh->Bind();
        // TODO: Move it to another place
        shader->BindUniformMatrix4x4f("MainProjectionMatrix", camera->ProjectionMatrix());
        shader->UpdateUniforms();
        
        GRenderer->Draw(mesh, material, shader);
        
        mesh->Unbind();
        if (texture)
        {
            texture->Unbind();
        }
        material->Unbind();
        shader->Unbind();
    }
    
    currentRenderTarget->Unbind();
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

void CRenderSystem::OnChangedEntity(IEntityPtr entity)
{
    ISystem::OnChangedEntity(entity);
    
    entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComponent)
    {
        std::map<CRenderComponentPtr, uint64_t>::iterator it = m_OrderKeys.find(renderComponent);
        if (it == m_OrderKeys.end())
        {
            return;
        }
        
        uint64_t order_key = SOrderComparator::OrderKey(renderComponent, entity);
        if (order_key != it->second)
        {
            it->second = order_key;
            m_SortedComponents.sort(SOrderComparator());
        }
    });
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
