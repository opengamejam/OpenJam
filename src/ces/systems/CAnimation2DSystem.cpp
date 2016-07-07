//
//  CRenderSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CAnimation2DSystem.h"
#include "IEntity.h"
#include "CAnimation2DComponent.h"
#include "CTransformationComponent.h"
#include "CRenderComponent.h"
#include "IMaterial.h"
#include "IShaderProgram.h"
#include "IVertexBuffer.h"
#include "IMesh.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CAnimation2DSystem::CAnimation2DSystem()
{    
    RegisterComponent<CAnimation2DComponent>();
}

CAnimation2DSystem::~CAnimation2DSystem()
{
    
}

void CAnimation2DSystem::Update(unsigned long dt)
{
    const ISystem::TEntities& entities = Entities();
    std::for_each(entities.begin(), entities.end(), [&](IEntityPtr entity)
    {
        if (!IsEntityAdded(entity))
        {
            return;
        }
        
        entity->Get<CAnimation2DComponent>([&](CAnimation2DComponentPtr anim2DComponent)
        {
            bool frameChanged = anim2DComponent->Time(anim2DComponent->Time() + dt);
            if (frameChanged || anim2DComponent->IsStatic())
            {
                // Update texture coords
                entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComponent)
                {
                    ITexturePtr currentTexture = renderComponent->Texture(anim2DComponent->TextureName());
                    if (currentTexture != renderComponent->Texture())
                    {
                        renderComponent->Texture(currentTexture);
                    }
                    
                    IVertexBufferPtr vertexBuffer = renderComponent->Mesh()->VertexBuffer();
                    if (vertexBuffer->HasStream(IVertexBuffer::TextureCoords))
                    {
                        IVertexBuffer::SVertexStream& textureCoord = vertexBuffer->Lock(IVertexBuffer::TextureCoords);
                        textureCoord.Set<glm::vec2>(0, anim2DComponent->TextureFrame());
                        vertexBuffer->Unlock(true);
                    }
                    
                    renderComponent->Dirty();
                });
                
                entity->Get<CTransformationComponent>([&](CTransformationComponentPtr transformComponent)
                {
                    CTransform3Df frameTransform = anim2DComponent->FrameTransform();
                    transformComponent->AddTransform(CTransformationComponent::Animation,
                                                     frameTransform,
                                                     false);
                    transformComponent->Dirty();
                });
            }
        });
    });
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************